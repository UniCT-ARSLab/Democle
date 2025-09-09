/*
 * context_collector.h
 */

#ifndef __CONTEXT_COLLECTOR_H
#define __CONTEXT_COLLECTOR_H

#include <map>
#include <string>

class ContextCollector {
    std::map<void *, std::string> _objects;
    void * engine;
public:
    ContextCollector(void * e) : engine(e) {};
    void add(void * ptr, std::string name) {
        //std::cout << "NEW " << ptr << std::endl;
        _objects[ptr] = name;
    };
    void clear(void * ptr) {
        //std::cout << "CLEAR " << ptr << std::endl;
        _objects.erase(ptr);
    }
    void show_heap_debug();
    void empty();
};

// #define HEAP_DEBUG_ADD(ptr,name) { HeapDebug::instance()->add(ptr,name); }
// #define HEAP_DEBUG_DEL(ptr) { HeapDebug::instance()->clear(ptr); }
// #define HEAP_DEBUG_SHOW() { HeapDebug::instance()->show_heap_debug(); }
// #define HEAP_DEBUG_CLEAR() { HeapDebug::instance()->empty(); }

#endif
