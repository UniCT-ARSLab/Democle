/*
 * agent.cpp
 */

#include "democle.h"
#include <cstdarg>

Agent::Agent(string _name) : name(_name)
{
    accepted_messages = new t_message_template[64];
    accepted_messages_index = 0;
    e = new Engine(this, name);
    DEMOCLE::instance()->register_agent(this);
}

void * thread_start(void * x)
{
    Agent * a = reinterpret_cast<Agent *>(x);
    //a->dump_accepted_messages();
    a->get_engine()->start();
    return nullptr;
}


void Agent::start()
{
    Engine::set_current(e);
#ifdef HAS_EMBEDDED
    xTaskCreate((TaskFunction_t)thread_start, name.c_str(), 8192, this, tskIDLE_PRIORITY, NULL);
    run();
#else
    run();
    main_thread = new thread(thread_start, this);
    main_thread->detach();
#endif
}

Engine & Agent::operator+(AtomicFormula b)
{
    //std::cout << "BEGIN Agent Operator +" << std::endl;
    (*e) + b;
    //std::cout << "END Agent Operator +" << std::endl;
    return *e;
}


Engine & Agent::operator-(AtomicFormula b)
{
    //std::cout << "BEGIN Agent Operator -" << std::endl;
    (*e) - b;
    //std::cout << "END Agent Operator -" << std::endl;
    return *e;
}

Engine & Agent::operator<<(AtomicFormula b)
{
    (*e) << b;
    return *e;
}


bool Agent::verify_message(AtomicFormula & b)
{
    for (int it = 0; it < accepted_messages_index; it++) {
        t_message_template * mt = &accepted_messages[it];
        //cout << "AM: " << mt->name << "," << mt->arity << endl;

        if ( !strcmp(mt->name, b.get_name().c_str()) && mt->arity == b.arity())
            return true;
    }
    return false;
}


void Agent::accept_messages(int x,...)
{

    va_list args;
    std::string bel_name;
    va_start(args, x);

    const char * c = va_arg(args, const char *);

    while (c != nullptr) {
        string s = c;
        int pos;
        if ( (pos = s.find("/")) == string::npos)
            throw BadMessageSyntaxException(s);
        bel_name = s.substr(0, pos);
        int arity = stoi(s.substr(pos+1));
        t_message_template mt;
        strcpy(mt.name,bel_name.c_str());
        mt.arity = arity;
        //accepted_messages.push_back(mt);
        accepted_messages[accepted_messages_index++] = mt;
        c = va_arg(args, const char *);
    }

    va_end(args);

    //dump_accepted_messages();
    // for (auto it = accepted_message_names.begin(); it != accepted_message_names.end(); it++) {
    //     cout << (*it) << endl;
    // }
}

void Agent::dump_accepted_messages()
{
    cout << "--------------------------------------------------------" << endl;
    cout << "Name : " << name << endl;
    cout << "--------------------------------------------------------" << endl;
    // for (auto it = accepted_messages.begin(); it != accepted_messages.end(); it++) {
    //     t_message_template mt = *it;
    //     cout << mt.name << "," << mt.arity << endl;
    // }
    for (auto it = 0; it < accepted_messages_index; it++) {
        t_message_template mt = accepted_messages[it];
        cout << mt.name << "," << mt.arity << endl;
    }
    cout << "--------------------------------------------------------" << endl;
}
