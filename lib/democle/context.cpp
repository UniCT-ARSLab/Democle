/*
 * context.cpp
 */

#include "context.h"
#include "engine.h"


Context::Context() : engine(nullptr),_sender("")
{
    //HEAP_DEBUG_ADD(this,"Context");
}

Context::Context(Context & c)
{
    engine = c.engine; variables = c.variables; _sender = c._sender;
    engine->get_collector()->add(this, "Context");
    //HEAP_DEBUG_ADD(this,"Context");
}


Context::~Context()
{
    engine->get_collector()->clear(this);
    //HEAP_DEBUG_DEL(this);
}

void Context::set_engine(Engine * e) {
    engine = e;
    e->get_collector()->add(this, "Context");
}


Context & Context::operator+(AtomicFormula b)
{
    //std::cout << "BEGIN Context Operator +" << std::endl;
    b.make_ground(this);
    (*engine) + b;
    //std::cout << "END Context Operator +" << std::endl;
    return (*this);
}

Context & Context::operator-(AtomicFormula b)
{
    //std::cout << "BEGIN Context Operator -" << std::endl;
    b.make_ground(this);
    (*engine) - b;
    //std::cout << "END Context Operator -" << std::endl;
    return (*this);
}

Context & Context::operator<<(AtomicFormula b)
{
    b.make_ground(this);
    // if the call is made inside a plan (thus using the context),
    // it is executed immediatelly and the event does not go in the event queue
    engine->execute_event(new Event(new AtomicFormula(b), call));
    //(*engine) << b;
    return (*this);
}


flexi_type & Context::operator[](term index)
{
    //std::cout << "Accessing variable " << index << std::endl;
    return variables[index.name()];
}

void Context::show_kb()
{
    engine->show_kb();
}

std::ostream& operator<<(std::ostream & out, Context & c)
{
    out << "Context " << (&c) ;
    for (std::map<std::string, flexi_type>::iterator it = c.variables.begin(); it != c.variables.end(); it++) {
        out << ": " << it->first << "=" << it->second;
    }
    out << endl;
    return out;
}

void show_context_vector(ContextPtrVector * p)
{
    for (ContextPtrVector::iterator it = p-> begin(); it != p->end(); it++) {
        Context * c = (*it);
        std::cout << (*c);
    }
}
