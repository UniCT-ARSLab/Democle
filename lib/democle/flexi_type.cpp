/*
 * flexi_type.cpp
 */

#include "flexi_type.h"
#ifdef HAS_EMBEDDED
#include <Arduino.h>
#endif

bool flexi_type::operator==(flexi_type & t)
{
    switch (_type) {
    case _int:
        return (int)(*this) == (int)t;
    case _float:
        return (float)(*this) == (float)t;
    case _string:
        return (string)(*this) == (string)t;
    default:
        return false;
    }
}

bool flexi_type::operator>(flexi_type & t)
{
    switch (_type) {
    case _int:
        return (int)(*this) > (int)t;
    case _float:
        return (float)(*this) > (float)t;
    case _string:
        return (string)(*this) > (string)t;
    default:
        return false;
    }
}

bool flexi_type::operator>=(flexi_type & t)
{
    switch (_type) {
    case _int:
        return (int)(*this) >= (int)t;
    case _float:
        return (float)(*this) >= (float)t;
    case _string:
        return (string)(*this) >= (string)t;
    default:
        return false;
    }
}

bool flexi_type::operator<(flexi_type & t)
{
    switch (_type) {
    case _int:
        return (int)(*this) < (int)t;
    case _float:
        return (float)(*this) < (float)t;
    case _string:
        return (string)(*this) < (string)t;
    default:
        return false;
    }
}

bool flexi_type::operator<=(flexi_type & t)
{
    switch (_type) {
    case _int:
        return (int)(*this) <= (int)t;
    case _float:
        return (float)(*this) <= (float)t;
    case _string:
        return (string)(*this) <= (string)t;
    default:
        return false;
    }
}

void flexi_type::set(flexi_type & t)
{
    switch (t._type) {
    case _int:
        set((int)t);
        break;
    case _float:
        set((float)t);
        break;
    case _string:
        set((string)t);
        break;
    default:
        break;
    }
}

ostream& operator<<(ostream& out, flexi_type & t)
{
#ifdef HAS_EMBEDDED
    switch (t._type) {
    case flexi_type::_int:
        Serial.print((int)t);
        break;
    case flexi_type::_float:
        Serial.print((float)t);
        break;
    case flexi_type::_string:
        Serial.print(((string)t).c_str());
        break;
    default:
        break;
    }
#else
    switch (t._type) {
    case flexi_type::_int:
        out << (int)t;
        break;
    case flexi_type::_float:
        out << (float)t;
        break;
    case flexi_type::_string:
        out << (string)t;
        break;
    default:
        break;
    }
#endif
    return out;
}


