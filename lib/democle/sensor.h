/*
 * sensor.h
 */

#ifndef __SENSOR_H
#define __SENSOR_H

#include <thread>
#include "agent.h"

class Sensor {
    thread * sensor_thread;
    void run();
 protected:
    Agent * agent;
 public:
    Sensor() : agent(nullptr) { };
    void start();
    void set_agent(Agent * a) { agent = a; };
    virtual void sense() = 0;
    friend void sensor_thread_proc(Sensor * s);
};


#endif
