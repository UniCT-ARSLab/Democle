/*
 * plan.cpp
 */

#include "plan.h"


ostream& operator<<(ostream & out, Plan & p)
{
    out << (*p.event) << " >> { ... }";
    return out;
}


// evt is the event got from the queue
bool Plan::match_event(Event * evt, Context * c)
{
    event->get_belief()->bind(c);
    return *event == *evt;
}

void Plan::bind(Context * c)
{
    event->get_belief()->bind(c);
}

void Plan::unbind()
{
    event->get_belief()->unbind();
}

void Plan::execute(Context * ctx)
{
    fun(*ctx);
}

