/*
 * democle.h
 */

#ifndef __DEMOCLE_H
#define __DEMOCLE_H

#ifdef STM32F4
#include <STM32FreeRTOS.h>
#endif

#include <string>
#include <cstdarg>

#include "democle_types.h"
#include "goal.h"
#include "plan.h"
#include "engine.h"
#include "agent.h"
#include "sensor.h"
#include "sensor_lib.h"
#include "url.h"
#include "protocol.h"
#include "packet.h"
#include "errors.h"

#include <map>

using namespace std;

class DEMOCLE {
    static DEMOCLE * _instance;
    std::map<string, Agent * > registry;
    std::map<string, AbstractProtocol * > protocol_registry;
 public:
    DEMOCLE();
    static DEMOCLE * instance() {
        if (_instance == nullptr) {
            _instance = new DEMOCLE();
        }
        return _instance;
    };
    void register_agent(Agent * a);
    Agent * get_agent(string name);
#ifndef HAS_EMBEDDED
    void _register_tcp_protocol(va_list args);
#endif
#if defined(HAS_EMBEDDED) && !defined(STM32F4)
    void _register_hermeslora_protocol(va_list args);
#endif
    void _send_message(Agent * sender, url & destination, AtomicFormula & b);
    void _put_message_in_queue(string & destination_agent, string & sender_agent, AtomicFormula & b);

    static void register_protocol(string protocol_name, ...) {
        va_list args;
        va_start(args, protocol_name);
#ifndef HAS_EMBEDDED
        if (protocol_name == "tcp")
            instance()->_register_tcp_protocol(args);
#endif
#if defined(HAS_EMBEDDED) && !defined(STM32F4)
        if (protocol_name == "hermeslora")
            instance()->_register_hermeslora_protocol(args);
#endif
        va_end(args);
    };

    static void send_message(Agent * sender, url & destination, AtomicFormula & b) {
        instance()->_send_message(sender, destination, b);
    };

    static void put_message_in_queue(string & destination_agent, string & sender_agent, AtomicFormula & b) {
        instance()->_put_message_in_queue(destination_agent, sender_agent, b);
    };
};


#endif
