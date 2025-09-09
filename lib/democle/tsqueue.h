/*
 * C++ thread-safe queue
 */
#ifdef STM32F4
#include <STM32FreeRTOS.h>

#define TSQUEUE_SIZE	128
// Thread-safe queue
template <typename T>
class TSQueue {
private:
	// Underlying queue
	QueueHandle_t m_queue;

public:
	TSQueue() {
		m_queue = xQueueCreate(TSQUEUE_SIZE, sizeof(T));
	};
	// Pushes an element to the queue
	void push(T item)
	{
		xQueueSend(m_queue, &item, portMAX_DELAY);
	}

	// Pops an element off the queue
	T pop()
	{
		// retrieve item
		T item;
		xQueueReceive(m_queue, &item, portMAX_DELAY);
		return item;
	}
};
#else
#include <mutex>
#include "lock.h"
#include <condition_variable>
#include <iostream>
#include <queue>

// Thread-safe queue
template <typename T>
class TSQueue {
private:
	// Underlying queue
	std::queue<T> m_queue;

	// mutex for thread synchronization
	democle_lock_t m_mutex;

	// Condition variable for signaling
	std::condition_variable m_cond;

public:
	// Pushes an element to the queue
	void push(T item)
	{

		// Acquire lock
		ENTER_CS(m_mutex);

		// Add item
		m_queue.push(item);

		// Notify one thread that
		// is waiting
		m_cond.notify_one();
		EXIT_CS(m_mutex);
	}

	// Pops an element off the queue
	T pop()
	{

		// acquire lock
		ENTER_CS(m_mutex);

		// wait until queue is not empty
		m_cond.wait(lock,
					[this]() { return !m_queue.empty(); });

		// retrieve item
		T item = m_queue.front();
		m_queue.pop();

		EXIT_CS(m_mutex);
		// return item
		return item;
	}
};
#endif
