#ifdef HELLO_AGENT_LOOP

#include <Arduino.h>

#include "democle.h"

procedure(say_hello);

class HelloAgent : public Agent {
public:
    HelloAgent() : Agent("hello_agent") {};
    void run() {

        var(N);

        say_hello(0) >> [](Context &c) {}; // do nothing
        say_hello(N) >> [N](Context & c)
        {
            int n = c[N];
            Serial.println("Hello world");
            c << say_hello(n-1);
        };

        this << say_hello(10);
    };
};


HelloAgent a;

void setup_agent() 
{
    a.start();
}

#endif

