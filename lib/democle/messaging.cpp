/*
 * messaging.cpp
 */

#include "democle.h"
#include "url.h"

MessageSender operator,(std::string dest, AtomicFormula b)
{
    //cout << "Sending message to " << dest << endl;
    return MessageSender(nullptr, dest, b);
    //b.set_sender(context->get_engine()->get_name());
    //(*a) + b;
    //return (*this);
}

MessageSender operator,(char * dest, AtomicFormula b)
{
    return MessageSender(nullptr, dest, b);
}

MessageSender operator,(const char * dest, AtomicFormula b)
{
    return MessageSender(nullptr, dest, b);
}

Context & Context::operator+(MessageSender msg)
{
    std::string destination = msg.get_destination();
    AtomicFormula & b = msg.get_belief();

    url u(destination);

    if (!u.is_good) {
        DEMOCLE::put_message_in_queue(destination, this->get_engine()->get_name(), b);
    }
    else {
        //cout << u.protocol << '\t' << u.host << "\t" << u.port << "\t" << u.path << endl;
        //cout << "Sending message from agent " << get_engine()->get_name() << " to remote agent " << destination << endl;
        Agent * a = engine->get_agent();
        DEMOCLE::send_message(a, u, b);
    }
    return (*this);
}

