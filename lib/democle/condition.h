/*
 * condition.h
 */

#ifndef __CONDITION_H
#define __CONDITION_H

#include <vector>
#include "democle_types.h"

using namespace std;

class Condition {
    vector<AtomicFormula> elements;
 public:
    Condition() { };
    Condition(const Condition & c) { elements = c.elements; };
    void append_belief(AtomicFormula & b) { elements.push_back(b); };
    Condition & operator&(AtomicFormula bel);
    bool eval(Engine * e, Context * starting_context, ContextPtrVector * & context_array);
    friend ostream& operator<<(ostream & out, Condition & c);
};


#endif
