/*
 * engine.h
 */

#ifndef __ENGINE_H
#define __ENGINE_H

#ifdef HAS_EMBEDDED
#include <Arduino.h>
#endif
#ifdef STM32F4
#include <STM32FreeRTOS.h>
#else
#include <thread>
#include <mutex>
#endif
#include "democle_types.h"
#include "goal.h"
#include "plan.h"
#include "tsqueue.h"
#include "context_collector.h"
#include "lock.h"

class Agent;

class KnowledgeBase {
    std::map<string, vector<AtomicFormula *>> kb;
    democle_lock_t m_mutex;
 public:
    KnowledgeBase() {
#ifdef STM32F4
        m_mutex = xSemaphoreCreateMutex();
        EXIT_CS(m_mutex);
#endif
    };
    bool assert_belief(AtomicFormula * bel);
    bool retract_belief(AtomicFormula * bel);
    vector<AtomicFormula *> * get_named_beliefs(string & name);
    bool get_matching_beliefs(AtomicFormula * bel, Context * starting_context, ContextPtrVector * ctx_array);
    bool verify_matching_beliefs(Context * ctx, AtomicFormula * bel);
    void show();
};


class Engine {
    KnowledgeBase kb;
    static Engine * current;
    vector<Plan*> plans;
    vector<Goal*> goals;
    TSQueue<Event*> event_queue;
    string name;
    Agent * agent;
    ContextCollector * collector;
    void run();
 public:
    Engine(Agent * a) {
        agent = a;
        current = this;
        name = to_string((long)this);
        collector = new ContextCollector(this);
    };
    Engine(Agent * a, string _nam) {
        agent = a;
        current = this;
        name = _nam;
        collector = new ContextCollector(this);
    };
    Engine & operator+(AtomicFormula b);
    Engine & operator-(AtomicFormula b);
    Engine & operator<<(AtomicFormula b);
    void add_plan(Plan * p) { plans.push_back(p); };
    void add_goal(Goal * p) { goals.push_back(p); };
    void show_kb() { kb.show(); };
    void show_plans();
    void start();
    void execute_event(Event * evt);

    string & get_name() { return name; };

    Agent * get_agent() { return agent; };

    ContextCollector * get_collector() { return collector; };

    KnowledgeBase * knowledge() { return &kb;};
    static Engine & get_current() { return *current; };
    static void set_current(Engine * e) { current = e; };

};



#endif

