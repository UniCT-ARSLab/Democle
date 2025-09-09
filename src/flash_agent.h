#ifdef FLASH_AGENT

#include <Arduino.h>

#include "democle.h"
#include "predicates.h"

#ifdef STM32F4
#define PIN PC13
#define LED PA5
#endif

#ifdef ESP32
#define PIN D9
#define LED D0
#endif


reactor(tick);
reactor(input_edge);
belief(state);

class FlashAgent : public Agent {
public:
    FlashAgent() : Agent("flash_agent") { };
    void run() {
        var(X);
        var(T);

        DigitalInputPoller * p = new DigitalInputPoller(PIN, FALLING, 100);
        pinMode(PIN, INPUT_PULLUP);
        attach(p);

        Timer * t1 = new Timer(500, "tim1");
        attach(t1);

        pinMode(LED, OUTPUT);

        +tick(string("tim1")) / state(0)  >> [](Context & c)
        {
            digitalWrite(LED,0);
        };

        +tick(string("tim1")) / state(1)  >> [](Context & c)
        {
            digitalWrite(LED,!digitalRead(LED));
        };

        +input_edge(PIN) / state(X) >> [X](Context & c)
        {
            int x = c[X];
            Serial.println("Button pressed");
            c - state(x);
            x = !x;
            c + state(x);
        };

        this + state(0);

    };
};

FlashAgent a;

void setup_agent()
{
    a.start();
}

#endif
