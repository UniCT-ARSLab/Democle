/*
 * goal.h
 */

#ifndef __GOAL_H
#define __GOAL_H

#include "democle_types.h"
#include "condition.h"

class Goal : public AtomicFormula {
    Condition * cond;
 public:
    Goal(term_vector & t) : AtomicFormula(t),cond(nullptr)  {  };
    Goal(const AtomicFormula & bel) : AtomicFormula(bel),cond(nullptr) { };
    Event operator+() = delete;
    Goal & operator<<(AtomicFormula b);
    Goal & operator<<(Condition c);
};

#define goal_preamble(name) \
 _##name(term_vector & t) : Goal(t) { create(); }

#define goal(name) \
    class _##name : public Goal { \
public:                          \
 goal_preamble(name) \
     virtual void create() { set_name(#name); };  \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }

#endif
