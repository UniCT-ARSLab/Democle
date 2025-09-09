#include <Arduino.h>

#include <iostream>
#include <cstdlib>

#include "democle.h"
#include "predicates.h"

#if defined(HAS_EMBEDDED) && defined(ESP32)

#include <esp_heap_caps.h>

void show_free_memory()
{
    uint32_t freeHeapBytes = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    uint32_t totalHeapBytes = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    // Print to serial
    Serial.printf("[Memory] %d of %d bytes free\n\r", freeHeapBytes, totalHeapBytes);
}

#endif

//#define BARE_AGENT
//#define REACTIVE_AGENT_1
//#define BUTTON_REACTIVE_AGENT
//#define PARKING_AGENT
//#define HELLO_AGENT
//#define HELLO_AGENT_LOOP
//#define CALC_MAX
#define FLASH_AGENT

#include "hello_agent.h"
#include "hello_agent_loop.h"
#include "bare_agent.h"
#include "reactive_agent1.h"
#include "button_reactive_agent.h"
#include "parking_agent.h"
#include "calc_max.h"
#include "flash_agent.h"


void setup() 
{
    Serial.begin(115200);
    delay(1000); // needed to setup Serial port

    setup_agent();
#ifdef STM32F4
    vTaskStartScheduler();
    Serial.println("RAM Error");
#endif
}

void loop() 
{
    delay(1000);
}
