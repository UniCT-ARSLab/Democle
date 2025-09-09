/*
 * plan.h
 */

#ifndef __PLAN_H
#define __PLAN_H

#include "democle_types.h"

class Plan {
    Event * event;
    action_fun fun;
public:
    Plan(Event * evt, action_fun bound_fun) : event(evt), fun(bound_fun) { };
    Condition * get_condition() { return event->get_condition(); };
    bool match_event(Event * evt, Context * c);
    void execute(Context * ctx);
    void bind(Context * ctx);
    void unbind();
    friend ostream& operator<<(ostream & out, Plan & p);
};


#endif
