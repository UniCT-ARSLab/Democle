/*
 * sensor.cpp
 */

#include "sensor.h"
#include <iostream>

void sensor_thread_proc(Sensor * s)
{
    s->run();
}

void Sensor::start()
{
#ifdef HAS_EMBEDDED
    xTaskCreate((TaskFunction_t)sensor_thread_proc, "Sensor", 4096, this, tskIDLE_PRIORITY, NULL);
#else
    sensor_thread = new thread(sensor_thread_proc, this);
    sensor_thread->detach();
#endif
}


void Sensor::run()
{
    while (true) {
        sense();
    }
}
