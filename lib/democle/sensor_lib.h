/*
 * sensor_lib.h
 */

#ifndef __SENSOR_LIB_H
#define __SENSOR_LIB_H

#include <map>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <utility>
#include "sensor.h"

class Timer : public Sensor {
    int mills;
    std::string name;
 public:
    Timer(int m, std::string _n) : Sensor(),mills(m),name(_n) { };
    virtual void sense();
};


#ifdef HAS_EMBEDDED

#include <Arduino.h>
#ifdef STM32F4
#include <STM32FreeRTOS.h>
#endif

class DigitalInputPoller : public Sensor {
    int pin, edge, time_delay;
    int old_state;
 public:
    DigitalInputPoller(int _pin, int _edge, int _del) : Sensor(),pin(_pin),edge(_edge),time_delay(_del) 
    { 
        pinMode(pin, INPUT);
        if (edge == FALLING)
            old_state = 1;
        else
            old_state = 0;
    };
    virtual void sense();
};


class AnalogInputPoller : public Sensor {
    int pin, period;
 public:
    AnalogInputPoller(int _pin, int _period) : Sensor(),pin(_pin),period(_period) 
    {
    };
    virtual void sense();
};


class DigitalInputEventHandler {
    int event_pin;
    std::map<int, AtomicFormula *> belief_map;
    std::map<int, Agent *> agent_map;
    std::map<int, bool> event;
    TaskHandle_t event_task;
    static DigitalInputEventHandler * _instance;
    static bool started;
public:
    DigitalInputEventHandler() : event_pin(-1) { };
    static DigitalInputEventHandler * instance() {
        if (_instance == nullptr)
            _instance = new DigitalInputEventHandler();
        return _instance;
    };
    void start();
    void add(int pin, AtomicFormula * f, Agent * a);
    void send_event(int pin);
    void run();
};


#define map_input_pin(pin, edge, b)                                     \
    {                                                                   \
        AtomicFormula bx = b();                                         \
        AtomicFormula * belief_##pin = new AtomicFormula(bx);           \
        DigitalInputEventHandler::instance()->add(pin, belief_##pin, this); \
        setup_digital_##pin(edge);                                      \
    }




#define use_input_pin(pin)                                              \
    void ISR_digital_##pin()                                            \
    {                                                                   \
        DigitalInputEventHandler::instance()->send_event(pin);          \
    }                                                                   \
                                                                        \
    void setup_digital_##pin(int edge)                                  \
    {                                                                   \
        pinMode(pin, INPUT);                                            \
        attachInterrupt(digitalPinToInterrupt(pin), ISR_digital_##pin, edge); \
    }

#endif

#endif

