#ifdef HELLO_AGENT

#include <Arduino.h>

#include "democle.h"

procedure(say_hello);

class HelloAgent : public Agent {
public:
    HelloAgent() : Agent("hello_agent") {};
    void run() {

        say_hello() >> [](Context & c)
        {
            Serial.println("Hello world");
        };

        this << say_hello();
    };
};


HelloAgent a;

void setup_agent() 
{
    a.start();
}

#endif

