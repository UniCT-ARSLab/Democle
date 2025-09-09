#ifdef REACTIVE_AGENT_1

#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

belief(parking_stall);

class ReactiveAgent : public Agent {
public:
    ReactiveAgent() : Agent("myreactiveagent") {};
    void run() {

        // define variable names
        var(StallNumber);

        // reacting to belief assertion
        + parking_stall(StallNumber) >> [StallNumber](Context & c)
        {
            // covert the DEMOCLE term into a C++ term
            std::string stall_num = c[StallNumber];
            Serial.printf("Added stall %s\n", stall_num.c_str());
        };

        // populate your KB
        this + parking_stall("A0");
        this + parking_stall("A1");
        this + parking_stall("A2");
        this + parking_stall("B3");

    };
};

ReactiveAgent a;

void setup_agent()
{
    a.start();
}

#endif

