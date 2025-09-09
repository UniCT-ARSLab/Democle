/*
 * packet.cpp
 */

#include "democle.h"
#include "packet.h"

#define MAGIC 0xDEAF0001

using namespace std;

DEMOCLEPacket::DEMOCLEPacket()
{
    stream << MAGIC;
}

DEMOCLEPacket::DEMOCLEPacket(uint8_t * ptr, int len)
{
    stream.fill(ptr, len);
    int magic_;
    stream >> magic_;
    if (magic_ != MAGIC)
        throw new InvalidMagicNumber;
}


void DEMOCLEPacket::put(AtomicFormula a)
{
    term_vector & t = a.get_terms();
    stream << a.get_name();
    stream << (int)a.is_reactor();
    stream << (int)t.size();
    for (int i = 0; i < t.size(); i++) {
        flexi_type & v = t[i].value();
        int _type = v._type;
        stream << _type;
        switch (_type) {
        case flexi_type::_int:
            stream << (int)v;
            break;
        case flexi_type::_float:
            stream << (double)v;
            break;
        case flexi_type::_string:
            string s = v;
            stream << s;
            break;
        }
    }
}

void DEMOCLEPacket::put(std::string s)
{
    stream << s;
}

void DEMOCLEPacket::put(int s)
{
    stream << s;
}

void DEMOCLEPacket::put(double s)
{
    stream << (double)s;
}

void DEMOCLEPacket::get(AtomicFormula & a)
{
    std::string n;
    int r;
    stream >> n;
    stream >> r;
    a.set_name(n);
    if (r)
        a.set_as_reactor();

    term_vector t;
    int term_count;
    stream >> term_count;
    for (int i = 0; i < term_count; i++) {
        int _type;
        term _term(0);
        stream >> _type;
        switch (_type) {
        case flexi_type::_int:
            {
                int v;
                stream >> v;
                _term.set(v);
            }
            break;
        case flexi_type::_float:
            {
                double v;
                stream >> v;
                _term.set(v);
            }
            break;
        case flexi_type::_string:
            {
                string v;
                stream >> v;
                _term.set(v);
            }
            break;
        }
        t.push_back(_term);
    }

    a.set_terms(t);
}

void DEMOCLEPacket::get(std::string & s)
{
    stream >> s;
}

void DEMOCLEPacket::get(int & s)
{
    stream >> s;
}
