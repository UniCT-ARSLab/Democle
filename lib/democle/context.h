/*
 * context.h
 */

#ifndef __CONTEXT_H
#define __CONTEXT_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "flexi_type.h"
#include "context_collector.h"

class Engine;
class AtomicFormula;
class term;
class Context;

class MessageSender {
 public:
    MessageSender(Context * ctx, std::string dest, AtomicFormula & bel) : destination(dest), context(ctx), belief(bel) { };
    //MessageSender & operator<<(AtomicFormula b);
    std::string & get_destination() { return destination; };
    AtomicFormula & get_belief() { return belief; };
 private:
    std::string destination;
    Context * context;
    AtomicFormula & belief;
};


typedef std::map<std::string, flexi_type> variable_map;

class Context{
    variable_map variables;
    Engine * engine;
    string _sender;
public:
    Context();
    Context(Context & c);
    ~Context();
    void set_engine(Engine * e);
    Engine * get_engine() { return engine; };
    flexi_type & operator[](string index) {
        return variables[index];
    };
    flexi_type & operator[](term index);
    bool has_var(string n) { return variables.count(n); };
    void show_kb();
    string & sender() { return _sender; };
    void set_sender(string & s) { _sender = s; };
    Context & operator+(AtomicFormula b);
    Context & operator+(MessageSender msg);
    Context & operator-(AtomicFormula b);
    Context & operator<<(AtomicFormula b);

    friend std::ostream& operator<<(std::ostream & out, Context & c);
};

MessageSender operator,(std::string dest, AtomicFormula b);
MessageSender operator,(char * dest, AtomicFormula b);
MessageSender operator,(const char * dest, AtomicFormula b);

std::ostream& operator<<(std::ostream & out, Context & c);

typedef vector<Context *> ContextPtrVector;

void show_context_vector(ContextPtrVector * p);

#endif
