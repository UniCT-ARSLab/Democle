/*
 * goal.cpp
 */

#include "goal.h"
#include "engine.h"

Goal & Goal::operator<<(AtomicFormula b)
{
    cond = new Condition();
    cond->append_belief(b);
    Engine::get_current().add_goal(this);
    return (*this);
}

Goal & Goal::operator<<(Condition c)
{
    cond = new Condition(c);
    Engine::get_current().add_goal(this);
    return (*this);
}

