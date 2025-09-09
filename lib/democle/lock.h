/*
 * lock.h
 */

#ifdef STM32F4

#include <STM32FreeRTOS.h>

#define ENTER_CS(m) \
    xSemaphoreTake( m, portMAX_DELAY )

typedef SemaphoreHandle_t  democle_lock_t;

#define EXIT_CS(m) \
    xSemaphoreGive( m )
#else

#define ENTER_CS(m) \
    std::unique_lock<std::mutex> lock(m);

#define EXIT_CS(m) {}

typedef std::mutex  democle_lock_t;

#endif
