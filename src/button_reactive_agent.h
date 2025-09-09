#ifdef BUTTON_REACTIVE_AGENT

#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

reactor(input_edge);

class ButtonReactiveAgent : public Agent {
public:
    ButtonReactiveAgent() : Agent("buttonreactiveagent") {};
    void run() {

        DigitalInputPoller * p = new DigitalInputPoller(D9, FALLING, 100);
        pinMode(D9, INPUT_PULLUP);
        attach(p);

        // reacting to belief assertion
        + input_edge(D9) >> [](Context & c)
        {
            Serial.printf("Button pressed!\n");
        };

    };
};

ButtonReactiveAgent a;

void setup_agent()
{
    a.start();
}

#endif

