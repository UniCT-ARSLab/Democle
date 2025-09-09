/*
 * context_collector.cpp
 */

#include <iostream>
#include "context_collector.h"

void ContextCollector::show_heap_debug()
{
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "CURRENT HEAP of engine " << engine << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    for (auto it = _objects.begin(); it != _objects.end(); it++) {
        void * ptr = it->first;
        std::string & name = it->second;
        std::cout << ptr << ":" << name << std::endl;
    }

    std::cout << "-----------------------------------------------" << std::endl;
}

#include "context.h"

void ContextCollector::empty()
{
    for (;;) {
        auto it = _objects.begin();
        if (it == _objects.end())
            break;
        void * ptr = it->first;
        //std::cout << "Deleting " << ptr << std::endl;
        delete (Context *)ptr;
    }
    //_objects.clear();
}
