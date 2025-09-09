#ifdef BARE_AGENT

#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

belief(parking_stall);

class BasicAgent : public Agent {
public:
    BasicAgent() : Agent("myagent") {};
    void run() {

        // define your behaviour here

        // define your basic beliefs
        this + parking_stall("A0");
        this + parking_stall("A1");
        this + parking_stall("A2");
        this + parking_stall("B3");

    };
};


BasicAgent a;

void setup_agent()
{
    a.start();
    a.show_kb();
}

#endif

