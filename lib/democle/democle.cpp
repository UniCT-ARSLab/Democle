/*
 * democle.cpp
 */

#include "democle.h"

DEMOCLE * DEMOCLE::_instance = nullptr;

DEMOCLE::DEMOCLE()
{
}

void DEMOCLE::register_agent(Agent * a)
{
    registry[a->get_name()] = a;
}

Agent * DEMOCLE::get_agent(string name)
{
    if (registry.count(name))
        return registry[name];
    else
        return nullptr;
}

#ifndef HAS_EMBEDDED
void DEMOCLE::_register_tcp_protocol(va_list args)
{
    char * host = va_arg(args, char *);
    int port = va_arg(args, int);
    protocol_registry["tcp"] = new TCPProtocol(host, port);
}
#endif

#if defined(HAS_EMBEDDED) && !defined(STM32F4)
void DEMOCLE::_register_hermeslora_protocol(va_list args)
{

    if (protocol_registry.find("hermeslora") != protocol_registry.end()) {
        //Serial.println("hermeslora protocol is already registered.");
        return; 
    }

    const char* devName = va_arg(args, const char*);
    int loraCS = va_arg(args, int);
    int loraIRQ = va_arg(args, int);
    int loraRST = va_arg(args, int);
    int loraIO1 = va_arg(args, int);

    int has_spi = va_arg(args, int);
    if (has_spi){
        float freq = va_arg(args, double);
        int power = va_arg(args, int);
        int spi_miso = va_arg(args, int);
        int spi_mosi = va_arg(args, int);
        int spi_clk = va_arg(args, int);
        protocol_registry["hermeslora"] = new HermesLoraProtocol(devName, loraCS, loraIRQ, loraRST, loraIO1, freq, power, spi_miso, spi_mosi, spi_clk);

    } else {
        protocol_registry["hermeslora"] = new HermesLoraProtocol(devName, loraCS, loraIRQ, loraRST, loraIO1);
    }
 
}
#endif

void DEMOCLE::_send_message(Agent * sender, url & destination, AtomicFormula & b)
{
    if (protocol_registry.count(destination.protocol)) {
        AbstractProtocol * p = protocol_registry[destination.protocol];
        p->send_message(sender, destination, b);
    }
    else
        throw new ProtocolNotStartedException();
}

void DEMOCLE::_put_message_in_queue(string & destination_agent, string & sender_agent, AtomicFormula & b)
{
    Agent * a = DEMOCLE::instance()->get_agent(destination_agent);

    if (a == nullptr)
    {
        Serial.println("Agent not found\n");
        return;
        // throw AgentNotFoundException(destination_agent);
    }

    //cout << "Sending message from agent " << sender_agent << " to agent " << a->get_name() << "," << b << endl;
    if (!a->verify_message(b))
        throw MessageNotAcceptedException();

    b.set_sender(sender_agent);
    (*a) + b;
}



