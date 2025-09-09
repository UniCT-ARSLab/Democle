#ifdef CALC_MAX

#include <Arduino.h>

#include "democle.h"
#include "predicates.h"

belief(number);
procedure(calc_max);

class CalcMax : public Agent {
public:
    CalcMax() : Agent("calc_max") {};
    void run() {

        var(X);
        var(M);

        calc_max() / number(X)  >> [X](Context & c)
        {
            int x = c[X];
            Serial.printf("Starting from %d\n", x);
            c << calc_max(X);
        };

        calc_max(M) / (number(X) & gt(X, M)) >> [X](Context & c)
        {
            int x = c[X];
            Serial.printf("Continuing with %d\n", x);
            c << calc_max(x);
        };

        calc_max(M) >> [M](Context & c)
        {
            int m = c[M];
            Serial.printf("Max is %d\n", m);
        };

#ifndef STM32F4
        srand(time(NULL));
#endif
        for (int i = 0; i < 30;i++) {
            int n = (int)(rand() * 3000.0 / RAND_MAX);
            this + number(n);
        }
        show_kb();
        this << calc_max();

    };
};


CalcMax a;

void setup_agent() 
{
    a.start();
}

#endif

