/*
 * packet.h
 */

#ifndef __PACKET_H
#define __PACKET_H

#include "democle.h"
#include "binary_stream.h"

class DEMOCLEPacket {
    BinaryStream stream;
 public:
    DEMOCLEPacket();
    DEMOCLEPacket(uint8_t * ptr, int len);
    void put(AtomicFormula a);
    void put(std::string s);
    void put(int i);
    void put(double i);
    void get(AtomicFormula & a);
    void get(std::string & s);
    void get(int & i);
    void dump() { stream.dump(); };
    char * data() { return (char *)stream.data(); };
    int size() { return stream.size(); };
};



#endif
