/*
 * engine.cpp
 */

//
// DEMOCLE: DEclarative Multi-agent Open C-Language Engine
//
#ifdef HAS_EMBEDDED
#include <Arduino.h>
#endif

#include "errors.h"
#include "engine.h"
#include "intention.h"
#include "sensor.h"
#include "debug.h"

#include <unistd.h>

#include "lock.h"
Engine * Engine::current = nullptr;

bool KnowledgeBase::assert_belief(AtomicFormula * bel)
{
    ENTER_CS(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            if (b->match_ground(*bel)) {
                EXIT_CS(m_mutex);
                return false;
            }
        }
        v.push_back(bel);
        EXIT_CS(m_mutex);
        return true;
    }
    else {
        vector<AtomicFormula*> v;
        v.push_back(bel);
        kb[bel->get_name()] = v;
        EXIT_CS(m_mutex);
        return true;
    }
}

bool KnowledgeBase::retract_belief(AtomicFormula * bel)
{
    ENTER_CS(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            if (b->match_ground(*bel)) {
                v.erase(it);
                delete b;
                EXIT_CS(m_mutex);
                return true;
            }
        }
    }
    EXIT_CS(m_mutex);
    return false;
}

vector<AtomicFormula *> * KnowledgeBase::get_named_beliefs(string & name)
{
    vector<AtomicFormula *> * ptr = NULL;
    ENTER_CS(m_mutex);
    if (kb.count(name))
        ptr = & kb[name];
    EXIT_CS(m_mutex);
    return ptr;

}

bool KnowledgeBase::get_matching_beliefs(AtomicFormula * bel, Context * starting_context, ContextPtrVector * ctx_array)
{
    ENTER_CS(m_mutex);
    bool ret_val = false;
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            Context * ctx = new Context(*starting_context);
            bel->bind(ctx);
            if (bel->match_and_bind(*b)) {
                DEBUG(cout << "KnowledgeBase::get_matching_beliefs TRUE" << endl;);
                ret_val = true;
                ctx_array->push_back(ctx);
            }
            else {
                DEBUG(cout << "KnowledgeBase::get_matching_beliefs FALSE" << endl;);
                delete ctx;
            }
            bel->unbind();
        }
    }
    EXIT_CS(m_mutex);
    return ret_val;
}

bool KnowledgeBase::verify_matching_beliefs(Context * ctx, AtomicFormula * bel)
{
    ENTER_CS(m_mutex);
    if (kb.count(bel->get_name())) {
        vector<AtomicFormula *> & v = kb[bel->get_name()];
        for (vector<AtomicFormula*>::iterator it = v.begin(); it != v.end(); it++) {
            AtomicFormula * b = (*it);
            bel->bind(ctx);
            if (bel->match_and_bind(*b)) {
                bel->unbind();
                EXIT_CS(m_mutex);
                return true;
            }
            bel->unbind();
        }
    }
    EXIT_CS(m_mutex);
    return false;
}

void KnowledgeBase::show()
{
    ENTER_CS(m_mutex);
    for (const auto& _pair  : kb) {
        auto &bel_vector = _pair.second;
        for (auto b : bel_vector ) {
            cout << *b;
#ifdef HAS_EMBEDDED
            Serial.println();
#else
            cout << endl;
#endif            
        }
    }
    EXIT_CS(m_mutex);
}


Engine & Engine::operator+(AtomicFormula b)
{
    //std::cout << "Engine::operator+ " << b << std::endl;
    //std::cout << "BEGIN Engine::operator+" << std::endl;
    if (b.is_reactor()) {
        //std::cout << "REACTOR" << std::endl;
        event_queue.push(new Event(new AtomicFormula(b), add));
        //std::cout << "END Engine::operator+" << std::endl;
        return *this;
    }

    AtomicFormula * f = new AtomicFormula(b);
    if (kb.assert_belief(f)) {
        event_queue.push(new Event(new AtomicFormula(b), add));
    }
    else
        delete f;

    //std::cout << "END Engine::operator+" << std::endl;
    return *this;
}

Engine & Engine::operator-(AtomicFormula b)
{
    //std::cout << "BEGIN Engine::operator-" << std::endl;
    if (b.is_belief()) {
        if (kb.retract_belief(&b)) {
            event_queue.push(new Event(new AtomicFormula(b), del));
        }
    }
    //std::cout << "END Engine::operator-" << std::endl;
    return *this;
}

Engine & Engine::operator<<(AtomicFormula b)
{
    if (b.is_procedure()) {
        event_queue.push(new Event(new AtomicFormula(b), call));
        return *this;
    }

    return *this;
}

void Engine::show_plans()
{
    for (vector<Plan*>::iterator it = plans.begin(); it != plans.end(); it++) {
        cout << (**it) << endl;
    }
}

void show_free_memory();


void Engine::run()
{
#ifndef HAS_EMBEDDED
    cout << "[" << name << "] Agent started" << endl;
#endif

    for (vector<Sensor *>::iterator it = agent->get_sensors().begin();
         it != agent->get_sensors().end();
         it++)
        {
            Sensor * s = (*it);
            s->start();
        }


    while (true) {

        Event * evt = event_queue.pop();
        DEBUG(cout << "BEGIN-------------------------------------------------" << endl;)

        DEBUG(cout << "Event queued : " << (*(evt->get_belief())) << endl;)

        execute_event(evt);

        delete evt;
        //cout << "END-------------------------------------------------" << endl;

        //HEAP_DEBUG_SHOW();
        //HEAP_DEBUG_CLEAR();

        collector->empty();
        //collector->show_heap_debug();

        //show_free_memory();

    }
}

void Engine::execute_event(Event * evt)
{
    vector<Intention *> intentions;

    for (vector<Plan*>::iterator it = plans.begin(); it != plans.end(); it++) {
        Plan * p = *it;
        p->unbind();
        DEBUG(cout << "   Testing " << (*p) << endl);
        Context * c = new Context();
        c->set_engine(this);
        if (p->match_event(evt, c)) {
            Condition * cond = p->get_condition();
            if (cond != nullptr) {
                //show_free_memory();
                ContextPtrVector * context_array = new ContextPtrVector;
                DEBUG(cout << "-->Evaluating Condition" << endl;);
                bool eval_result = cond->eval(this, c, context_array);
                //show_context_vector(context_array);
                DEBUG(cout << "-->" << endl;);
                if (!eval_result || context_array->size() == 0) {
                    DEBUG(cout << "COND FALSE" << endl;);
                    delete c;
                    delete context_array;
                    continue;
                }
                DEBUG(cout << "COND true" << endl;);
                ContextPtrVector::iterator it = context_array->begin();
                Context * new_c = (*it);
                it++;
                for (; it != context_array->end(); it++) {
                    Context * c_to_delete = (*it);
                    DEBUG(cout << "DELETING context " << c_to_delete << endl;);
                    delete c_to_delete;
                }
                delete context_array;
                DEBUG(cout << "   Candidate Intention for the event " << (*p) << endl;);
                DEBUG(cout << (*c) << endl;);
                //show_free_memory();
                intentions.push_back(new Intention(p, new_c));
            }
            else {
                DEBUG(cout << "   Candidate Intention for the event " << (*p) << endl;);
                DEBUG(cout << (*c) << endl;);
                intentions.push_back(new Intention(p, c));
            }
        }
        else {
            delete c;
        }
    }

    for (vector<Intention *>::iterator it = intentions.begin(); it != intentions.end(); it++) {
        Intention * i = (*it);
        Plan * p = i->plan();
        Context * c = i->context();
        c->set_engine(this);
        c->set_sender(evt->get_belief()->get_sender());
        DEBUG(cout << "   Executing Intention " << (*p) << endl;);
        p->bind(c);
        bool success = true;
        try {
            p->execute(c);
        }
        catch (CutPlanException ex) {
            success = false;
        }
        p->unbind();
        if (success)
            break;

        //cout << "   Unbound plan "  << (*p) << endl;
    }
    for (vector<Intention *>::iterator it = intentions.begin(); it != intentions.end(); it++) {
        Intention * i = *it;
        Plan * p = i->plan();
        Context * c = i->context();
        p->unbind();
        delete c;
        delete i;
    }
}

void Engine::start()
{
    run();
}

