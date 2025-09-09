/*
 * democle_types.h
 */

#ifndef __DEMOCLE_TYPES_H
#define __DEMOCLE_TYPES_H

#include <iostream>
#include <vector>
#include <functional>
#include "flexi_type.h"
#include "context.h"

using namespace std;

class term {
    flexi_type val;
    Context * ctx;
    string _name;
 public:
    term(int t) : val(t), ctx(nullptr), _name(""), term_type(ground) { };
    term(string t) : val(t), ctx(nullptr), _name(""), term_type(ground) { };
    term(const char * t) : val(t), ctx(nullptr), _name(""), term_type(ground) { };
    term(float t) : val(t), ctx(nullptr), _name(""), term_type(ground) { };
    term(double t) : val(t), ctx(nullptr), _name(""), term_type(ground) { };
    term(bool v, const char * n) : ctx(nullptr), _name(n), term_type(variable) { };
    string& name() { return _name; };
    flexi_type & value();
    void set(term new_val);
    bool is_bound();
    int type() { return term_type; };
    bool is_const() { return term_type == ground; };
    bool is_var() { return term_type == variable; };
    void make_ground();

    void unbind() { ctx = nullptr; };
    void bind(Context * c) { ctx = c; };

    virtual bool operator!=(term & t) { return !(*this == t); };
    virtual bool operator==(term & t);
    virtual bool operator>(term & t);
    virtual bool operator>=(term & t);
    virtual bool operator<(term & t);
    virtual bool operator<=(term & t);

    enum { ground, variable } term_type;

    friend ostream& operator<<(ostream& out, term & t);
};

ostream& operator<<(ostream& out, term & t);


typedef vector<term> term_vector;
term_vector & mk_term(term_vector & terms);

template <typename H, typename... T> term_vector & mk_term(term_vector & terms, H head, T... tail)
{
    terms.push_back(head);
    return mk_term(terms, tail...);
}

#define bel_preamble(name) \
 _##name(term_vector & t) : AtomicFormula(t) { create(); }


#define belief(name) \
    class _##name : public AtomicFormula { \
public:                          \
 bel_preamble(name) \
     virtual void create() { set_name(#name); };  \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }


#define reactor_preamble(name) \
    _##name(term_vector & t) : AtomicFormula(t) { set_as_reactor(); create(); }

#define reactor(name)               \
    class _##name : public AtomicFormula { \
public:                          \
 reactor_preamble(name) \
     virtual void create() { set_name(#name); };  \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }


#define predicate_preamble(name,proc)                                       \
    _##name(term_vector & t) : AtomicFormula(t) { set_as_predicate(); create(); bool_fun = _pred_fun_##name; }

#define predicate(name, proc)            \
    bool _pred_fun_##name(term_vector  args) proc; \
    class _##name : public AtomicFormula { \
public:                          \
 predicate_preamble(name,proc)                      \
    virtual void create() { set_name(#name); }; \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }

#define procedure_preamble(name)                                       \
    _##name(term_vector & t) : AtomicFormula(t) { set_as_procedure(); create(); }

#define procedure(name)            \
    class _##name : public AtomicFormula { \
public:                          \
 procedure_preamble(name)                      \
    virtual void create() { set_name(#name); }; \
    }; \
    template <typename... Targs> _##name name(Targs... args) { \
        term_vector tv;                                        \
        return _##name(mk_term(tv, args...)) ;                 \
    }


#define var(V) term V(true, #V);



class AtomicFormula;
class Event;
class Plan;
class Condition;

#include "condition.h"

typedef function<void(Context & ctx)> action_fun;
typedef function<bool(term_vector & t)> predicate_fun;

typedef enum  { add, del, call } event_type;

class Event {
    AtomicFormula * bel;
    event_type type;
    Condition * cond;
public:
    Event(AtomicFormula * b, event_type t);
    Event(Event & evt);
    ~Event();
    Condition * get_condition() { return cond; };
    Plan * operator>>(action_fun fun);
    Event& operator/(AtomicFormula b);
    Event& operator/(Condition b);
    AtomicFormula * get_belief() { return bel; };
    bool operator==(Event & evt);
    friend ostream& operator<<(ostream & out, Event & ev);
};


class AtomicFormula {
public:
    static int new_count;

    AtomicFormula(term_vector & t)  {
        _terms = t; ctx = nullptr; bel_type = belief_type; sender = "";
        //std::cout << "Creating AtomicFormula " << this << ", " << new_count << std::endl;
        ++new_count;
    };

    AtomicFormula(const AtomicFormula & bel) {
        name = bel.name; _terms = bel._terms; ctx = bel.ctx; bel_type = bel.bel_type; bool_fun = bel.bool_fun; sender = bel.sender;
        //std::cout << "Creating AtomicFormula " << this << ", " << new_count << std::endl;
        ++new_count;
    };

    ~AtomicFormula() {
        --new_count;
        //std::cout << "DELETING AtomicFormula " << this << ", " << new_count << std::endl;
    };

    void set_as_predicate() { bel_type = predicate_type; };
    void set_as_reactor() { bel_type = reactor_type; };
    void set_as_procedure() { bel_type = procedure_type; };

    bool is_reactor() { return bel_type == reactor_type; };
    bool is_predicate() { return bel_type == predicate_type; };
    bool is_procedure() { return bel_type == procedure_type; };
    bool is_belief() { return bel_type == belief_type; };

    virtual void set_name(string n) { name = n; };
    string & get_name() { return name; };

    term_vector & get_terms() { return _terms; };
    void set_terms(term_vector & t) { _terms = t; };
    int arity() { return _terms.size(); };

    bool match_ground(AtomicFormula & b);
    bool match_and_bind(AtomicFormula & b);
    bool match_free(AtomicFormula & b);

    void make_ground(Context * c);
    void unbind();
    void bind(Context * c);
    Context * context() { return ctx; };

    bool evaluate(Context * c);

    Event operator+();
    Event operator-();
    bool operator==(AtomicFormula & bel);
    Plan * operator>>(action_fun fun);
    Event operator/(AtomicFormula b);
    Event operator/(Condition b);

    Condition operator&(AtomicFormula bel);

    void set_sender(string & s) { sender = s;};
    string & get_sender() { return sender; };

    friend ostream& operator<<(ostream & out, AtomicFormula & b);

    enum { belief_type, reactor_type, predicate_type, procedure_type } bel_type;

    predicate_fun bool_fun;
private:
    term_vector _terms;
    string name;
    Context * ctx;
    string sender;
};

ostream& operator<<(ostream & out, AtomicFormula & b);
ostream& operator<<(ostream & out, Event & ev);



#endif
