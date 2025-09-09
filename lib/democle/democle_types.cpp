/*
 * democle_types.cpp
 */

#ifdef HAS_EMBEDDED
#include <Arduino.h>
#endif

#include "errors.h"
#include "democle_types.h"
#include "plan.h"
#include "engine.h"
#include "debug.h"

int AtomicFormula::new_count = 0;

term_vector & mk_term(term_vector & terms)
{
    return terms;
}


ostream& operator<<(ostream& out, term & t)
{
    switch (t.type()) {
    case term::ground:
        out << t.value();
        break;
    case term::variable:
        Context * ctx = t.ctx;
        string & n = t.name();
        if (ctx != nullptr && ctx->has_var(n))
            out << n << "(" << (*ctx)[n] << ")";
        else
            out << "?" << n;
    }
    return out;
}


ostream& operator<<(ostream & out, AtomicFormula & b)
{
#ifdef HAS_EMBEDDED
    Serial.printf("%s(",b.get_name().c_str());
#else
    out << b.get_name() << "(";
#endif
    term_vector & t = b.get_terms();

    for (int i = 0; i < t.size(); i++) {
        t[i].bind(b.ctx);
        out << t[i];
        t[i].unbind();
        if (i < t.size() - 1)
#ifdef HAS_EMBEDDED
    Serial.print(",");
#else
            out << ",";
#endif
    }
#ifdef HAS_EMBEDDED
    Serial.print(")");
#else
    out << ")";
#endif
    return out;
}


ostream& operator<<(ostream & out, Event & ev)
{
    switch (ev.type) {
        case add:
            out << '+';
            break;
        case del:
            out << '-';
            break;
    }
    out << (*ev.bel);
    if (ev.cond != nullptr) {
        out << " / ";
        cout << (*ev.cond);
    }
    return out;
}


flexi_type & term::value()
{
    if (!is_bound())
        throw UnboundVariableException(_name);

    if (term_type == term::ground)
        return val;
    else
        return (*ctx)[_name];
}

void term::make_ground()
{
    if (term_type == term::variable) {
        val = (*ctx)[_name];
        term_type = term::ground;
    }
}

void term::set(term new_term)
{
    if (term_type == term::ground)
        val = new_term.value();
    else {
        if (ctx == nullptr)
            throw NoContextException();
        else
            (*ctx)[_name] = new_term.value();
    }
}

bool term::is_bound()
{
    if (term_type == term::ground)
        return true;

    if (ctx == nullptr)
        return false;

    if (ctx->has_var(_name))
        return true;
    else
        return false;
}

bool term::operator==(term & t)
{
    if (!is_bound() || !t.is_bound())
        return false;
    else
        return value() == t.value();
}

bool term::operator>(term & t)
{
    if (!is_bound() || !t.is_bound())
        return false;
    else
        return value() > t.value();
}


bool term::operator>=(term & t)
{
    if (!is_bound() || !t.is_bound())
        return false;
    else
        return value() >= t.value();
}


bool term::operator<(term & t)
{
    if (!is_bound() || !t.is_bound())
        return false;
    else
        return value() < t.value();
}


bool term::operator<=(term & t)
{
    if (!is_bound() || !t.is_bound())
        return false;
    else
        return value() <= t.value();
}


bool AtomicFormula::match_ground(AtomicFormula & b)
{
    if (name != b.get_name())
        return false;
    term_vector & t2 = b.get_terms();
    if (t2.size() != _terms.size())
        return false;
    for (int i = 0; i < _terms.size();i++) {
        if (t2[i] != _terms[i])
            return false;
    }
    return true;
}


// TO DO
bool AtomicFormula::match_and_bind(AtomicFormula & b)
{
    DEBUG(cout << "AtomicFormula::match_and_bind "<< (*this) << " vs " << b << endl;)
    if (name != b.get_name())
        return false;
    term_vector & t2 = b.get_terms();
    if (t2.size() != _terms.size())
        return false;
    for (int i = 0; i < _terms.size();i++) {
        // both const
        if (_terms[i].is_const() && t2[i].is_const()) {
            if (_terms[i] != t2[i])
                return false;
        }

        if (_terms[i].is_const() && t2[i].is_var()) {
            string & name = t2[i].name();
            if (!ctx->has_var(name)) {
                throw UnboundVariableException(name);
            }
            flexi_type & ft1 = _terms[i].value();
            flexi_type & ft2 = (*ctx)[name];
            if (ft1 != ft2)
                return false;
        }

        if (_terms[i].is_var() && t2[i].is_const()) {
            string & name = _terms[i].name();
            if (!ctx->has_var(name)) {
                // the ith element of terms is a free variable
                (*ctx)[name] = t2[i].value();
            }
            else {
                flexi_type & ft1 = (*ctx)[name];
                flexi_type & ft2 = t2[i].value();
                if (ft1 != ft2)
                    return false;
            }
        }

        if (_terms[i].is_var() && t2[i].is_var()) {
            string & name1 = _terms[i].name();
            string & name2 = t2[i].name();
            if (!ctx->has_var(name2)) {
                throw UnboundVariableException(name);
            }
            if (!ctx->has_var(name1)) {
                // the ith element of terms is a free variable
                (*ctx)[name1] = (*ctx)[name2];
            }
            else {
                flexi_type & ft1 = (*ctx)[name1];
                flexi_type & ft2 = (*ctx)[name2];
                if (ft1 != ft2)
                    return false;
            }
        }

    }
    return true;
}


// matches current belief with b
bool AtomicFormula::match_free(AtomicFormula & b)
{
    // cout << (*this) << " vs " << b << endl;
    if (name != b.get_name())
        return false;
    term_vector & t2 = b.get_terms();
    if (t2.size() != _terms.size())
        return false;
    for (int i = 0; i < _terms.size();i++) {
        if (!_terms[i].is_bound() && !t2[i].is_bound()) {
            return false;
        }
        else if (_terms[i].is_bound() && t2[i].is_bound()) {
            if (_terms[i] != t2[i])
                return false;
        }
        // else {
        //     one is free and one is bound, so matching is possibile
        // }

    }
    return true;
}


void AtomicFormula::make_ground(Context * c)
{
    for (auto it = _terms.begin(); it != _terms.end(); it++) {
        it->bind(c);
        it->make_ground();
        it->unbind();
    }
}

void AtomicFormula::unbind()
{
    for (auto it = _terms.begin(); it != _terms.end(); it++) {
        it->unbind();
    }
}


void AtomicFormula::bind(Context * c)
{
    ctx = c;
    DEBUG(cout << "AtomicFormula::bind Binding context " << c << " to belief " << this << " " << (*this) << endl;)
    for (auto it = _terms.begin(); it != _terms.end(); it++) {
        it->bind(c);
    }
}

bool AtomicFormula::evaluate(Context * c)
{
    if (bel_type != predicate_type)
        return false;
    term_vector args;
    for (auto it = _terms.begin(); it != _terms.end(); it++) {
        term t = (*it);
        args.push_back(t);
    }
    bool b = bool_fun(args);
    DEBUG(cout << "PREDICATE " << b << endl;);
    return b;
}


Event AtomicFormula::operator+()
{
    if (!is_belief() && !is_reactor())
        throw NotABeliefException();
    Event e(new AtomicFormula(*this), add);
    return e;
}

Event AtomicFormula::operator-()
{
    if (!is_belief() && !is_reactor())
        throw NotABeliefException();
    Event e(new AtomicFormula(*this), del);
    return e;
}

bool AtomicFormula::operator==(AtomicFormula & bel)
{
    return match_and_bind(bel);
}

Plan * AtomicFormula::operator>>(action_fun fun)
{
    if (!is_procedure())
        throw NotAProcedureException();
    return Event(new AtomicFormula(*this), call) >> fun;
}

Event AtomicFormula::operator/(AtomicFormula b)
{
    if (!is_procedure())
        throw NotAProcedureException();
    return Event(new AtomicFormula(*this), call) / b;
}


Event AtomicFormula::operator/(Condition b)
{
    if (!is_procedure())
        throw NotAProcedureException();
    return Event(new AtomicFormula(*this), call) / b;
}

Condition AtomicFormula::operator&(AtomicFormula bel)
{
    if (!bel.is_belief() && !bel.is_predicate())
        throw NotABeliefException();
    Condition c;
    c.append_belief(*this);
    c.append_belief(bel);
    return c;
}

Event::Event(AtomicFormula * b, event_type t) : bel(b),type(t),cond(nullptr)
{
    //cout << "Creating event " << this << "," << b << endl;
    DEBUG(cout << (*bel) << endl;)
}

Event::Event(Event & evt)
{
    //cout << "Creating event " << this << endl;
    bel = new AtomicFormula(*evt.bel);
    type = evt.type;
    cond = evt.cond;
}

Event::~Event()
{
    if (bel != nullptr) {
        //cout << "Deleting event and bel " << this << "," << bel << endl;
        delete bel;
        bel = nullptr;
    }
}

bool Event::operator==(Event & evt)
{
    bool ret = (type == evt.type && *bel == *evt.bel);

    DEBUG(cout << "============================" << endl << "Event match " << endl;
          cout << (*this) << " vs " << evt << " = " << ret << endl;
          cout << (*bel->context()) << endl;
          cout << "============================" << endl;)

    return ret;
}

Event& Event::operator/(AtomicFormula b)
{
    cond = new Condition();
    cond->append_belief(b);
    return (*this);
}

Event& Event::operator/(Condition c)
{
    cond = new Condition(c);
    return (*this);
}

Plan * Event::operator>>(action_fun fun)
{
    Plan * p = new Plan(new Event(*this), fun);
    Engine::get_current().add_plan(p);
    return p;
}


