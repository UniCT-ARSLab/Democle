/*
 * intention.h
 */

#ifndef __INTENTION_H
#define __INTENTION_H

#include "plan.h"
#include <iostream>

class Intention {
    Plan * pln;
    Context * ctx;
 public:
    Intention(Plan * p, Context * c) : pln(p), ctx(c) {
        //std::cout << "Creating Intention " << this << std::endl;
    };
    ~Intention() {
        //std::cout << "Deleting Intention " << this << std::endl;
    };
    Context * context() { return ctx; };
    Plan * plan() { return pln; };
};

#endif
