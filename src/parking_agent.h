#ifdef PARKING_AGENT

#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

enum { STALL_FREE, STALL_BUSY };

#ifdef STM32F4
#define PIN PC13
#endif

#ifdef ESP32
#define PIN D9
#endif


belief(parking_stall);
belief(stall_state);
reactor(input_edge);

class ParkingAgent : public Agent {
public:
    ParkingAgent() : Agent("ParkingAgent") {};
    void run() {

        var(StallNum);

        DigitalInputPoller * p = new DigitalInputPoller(PIN, FALLING, 100);
        pinMode(PIN, INPUT_PULLUP);
        attach(p);

        + parking_stall(StallNum) >> [StallNum](Context & c)
        {
            // covert the DEMOCLE term into a C++ term
            std::string stall_num = c[StallNum];
            Serial.printf("Added stall %s\n", stall_num.c_str());
            c + stall_state(stall_num, STALL_FREE);
        };

        + input_edge(PIN) / (parking_stall(StallNum) & stall_state(StallNum, STALL_FREE)) >> \
            [StallNum] (Context & c)
        {
            std::string stall_num = c[StallNum];
            Serial.printf("Assing car to park stall %s\n", stall_num.c_str());
            c - stall_state(stall_num, STALL_FREE);            
            c + stall_state(stall_num, STALL_BUSY);            
        };

        + input_edge(PIN) >> [] (Context & c)
        {
            Serial.printf("No space left!\n");
        };

        // populate your KB
        this + parking_stall("A0");
        this + parking_stall("A1");
        this + parking_stall("A2");
        this + parking_stall("B0");
        this + parking_stall("B1");
        this + parking_stall("B2");

    };
};

ParkingAgent a;

void setup_agent()
{
    a.start();
}

#endif

