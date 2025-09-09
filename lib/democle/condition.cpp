/*
 * condition.cpp
 */

#include "condition.h"
#include "engine.h"
#include "errors.h"
#include "debug.h"

ostream& operator<<(ostream & out, Condition & c)
{
    bool f = true;
    for (vector<AtomicFormula>::iterator it = c.elements.begin(); it != c.elements.end(); it++) {
        AtomicFormula & b = (*it);
        if (f) {
            out << "(" << b;
            f = false;
        }
        else
            out << " & " << b;
    }
    out << ")";

    return out;
}


Condition & Condition::operator&(AtomicFormula bel)
{
    if (!bel.is_belief() && !bel.is_predicate())
        throw NotABeliefException();
    append_belief(bel);
    return (*this);
}


bool Condition::eval(Engine * e, Context * starting_context, ContextPtrVector * & context_array)
{
    KnowledgeBase * kb = e->knowledge();

    vector<AtomicFormula>::iterator it = elements.begin();
    AtomicFormula b = (*it);

    DEBUG(cout << "COND: checking belief " << b << endl;);
    if (b.is_predicate()) {
        b.bind(starting_context);
        if (!b.evaluate(starting_context)) {
            b.unbind();
            return false;
        }
        b.unbind();
    }
    else {
        if (!kb->get_matching_beliefs(&b, starting_context, context_array))
            return false;
    }

    it++;

    if (it == elements.end())
        return true;

    DEBUG(show_context_vector(context_array););
    for (; it != elements.end(); it++) {
        AtomicFormula b = (*it);
        DEBUG(cout << "COND: checking belief " << b << endl;);
        int idx = 0;

        ContextPtrVector * next_context_array = new ContextPtrVector;

        bool context_matched = false;

        for (ContextPtrVector::iterator ctx_it = context_array->begin(); ctx_it != context_array->end();) {

            Context * ctx = (*ctx_it);

            DEBUG(cout << idx << ":USING CONTEXT " << (*ctx) ;);

            bool match = true;

            if (b.is_predicate()) {
                b.bind(ctx);
                if (b.evaluate(ctx)) {
                    next_context_array->push_back(ctx);
                }
                else
                    match = false;
                b.unbind();
            }
            else {
                if (!kb->get_matching_beliefs(&b, ctx, next_context_array))
                    match = false;
            }

            if (match) {
                ctx_it = context_array->erase(ctx_it);
                context_matched = true;
            }
            else
                ctx_it++;

            idx++;
        }

        DEBUG(show_context_vector(context_array););
        for (auto ctx_it = context_array->begin(); ctx_it != context_array->end();ctx_it++) {
            Context * ctx = (*ctx_it);
            DEBUG(cout << "Deleting " << ctx << endl;);
            delete ctx;
        }
        delete context_array;
        context_array = next_context_array;
        DEBUG(show_context_vector(next_context_array););

        if (!context_matched)
            return false;
    }

    return true;
}
