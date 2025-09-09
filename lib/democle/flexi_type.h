/*
 * flexi_type.h
 */

#ifndef __FLEXI_TYPE_H
#define __FLEXI_TYPE_H

#include <iostream>
#include <string>

using namespace std;

class flexi_type {
protected:
    int value;
    double fvalue;
    string svalue;
public:
    flexi_type() : _type(no_type) { };
    flexi_type(int v) : value(v), _type(_int) { };
    flexi_type(float v) : fvalue(v), _type(_float) { };
    flexi_type(double v) : fvalue(v), _type(_float) { };
    flexi_type(string v) : svalue(v), _type(_string) { };
    flexi_type(const char * v) : svalue(v), _type(_string) { };

    virtual operator int() const { return value; };
    virtual operator string() const { return svalue; };
    virtual operator float() const { return (float)fvalue; };
    virtual operator double() const { return fvalue; };

    virtual void set(int val) { value = val; _type = _int; };
    virtual void set(float val) { fvalue = val; _type = _float;  };
    virtual void set(double val) { fvalue = val; _type = _float;  };
    virtual void set(const char * val) { svalue = val; _type = _string;  };
    virtual void set(string val) { svalue = val; _type = _string;  };

    virtual void set(flexi_type & t);

    virtual bool operator!=(flexi_type & t) { return !(*this == t); };
    virtual bool operator==(flexi_type & t);
    virtual bool operator>(flexi_type & t);
    virtual bool operator>=(flexi_type & t);
    virtual bool operator<(flexi_type & t);
    virtual bool operator<=(flexi_type & t);

    enum { no_type, _int, _float, _string } _type;

    friend ostream& operator<<(ostream& out, flexi_type & t);
};

ostream& operator<<(ostream& out, flexi_type & t);

#endif
