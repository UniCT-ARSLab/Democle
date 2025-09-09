/*
 * binary_stream.h
 */

#ifndef __BINARY_STREAM
#define __BINARY_STREAM

#include <iostream>
#include <string>
#include <stdint.h>
#include <string.h>

#define STREAM_SIZE 256

class BinaryStream {
    int len;
    uint8_t buffer[STREAM_SIZE];
 public:
    BinaryStream() : len(0) { };
    int size() { return len; };
    uint8_t * data() { return buffer; };
    void dump() {
        for (auto i = 0; i < len;i++) {
            std::cout << std::hex << (int)buffer[i] << std::dec << " ";
        }
        std::cout << std::endl;
    };
    void fill(uint8_t * ptr, int len)
    {
        memcpy(buffer, ptr, len);
    };

    BinaryStream & operator<<(char c) {
        buffer[len++] = (uint8_t)c;
        return *this;
    };
    BinaryStream & operator<<(int v) {
        uint8_t * ptr = (uint8_t *)&v;
        buffer[len++] = ptr[0];
        buffer[len++] = ptr[1];
        buffer[len++] = ptr[2];
        buffer[len++] = ptr[3];
        return *this;
    };
    BinaryStream & operator<<(unsigned int v) {
        uint8_t * ptr = (uint8_t *)&v;
        buffer[len++] = ptr[0];
        buffer[len++] = ptr[1];
        buffer[len++] = ptr[2];
        buffer[len++] = ptr[3];
        return *this;
    };
    BinaryStream & operator<<(double v) {
        uint8_t * ptr = (uint8_t *)&v;
        buffer[len++] = ptr[0];
        buffer[len++] = ptr[1];
        buffer[len++] = ptr[2];
        buffer[len++] = ptr[3];
        buffer[len++] = ptr[4];
        buffer[len++] = ptr[5];
        buffer[len++] = ptr[6];
        buffer[len++] = ptr[7];
        return *this;
    };
    BinaryStream & operator<<(std::string & s) {
        (*this) << (int)s.length();
        for (auto i = 0; i < s.length();i++)
            buffer[len++] = s.at(i);
        return *this;
    };

    BinaryStream & operator>>(char & c) {
        c = buffer[len++];
        return *this;
    };
    BinaryStream & operator>>(int & v) {
        uint8_t * ptr = (uint8_t *)&v;
        ptr[0] = buffer[len++];
        ptr[1] = buffer[len++];
        ptr[2] = buffer[len++];
        ptr[3] = buffer[len++];
        return *this;
    };
    BinaryStream & operator>>(unsigned int & v) {
        uint8_t * ptr = (uint8_t *)&v;
        ptr[0] = buffer[len++];
        ptr[1] = buffer[len++];
        ptr[2] = buffer[len++];
        ptr[3] = buffer[len++];
        return *this;
    };
    BinaryStream & operator>>(double & v) {
        uint8_t * ptr = (uint8_t *)&v;
        ptr[0] = buffer[len++];
        ptr[1] = buffer[len++];
        ptr[2] = buffer[len++];
        ptr[3] = buffer[len++];
        ptr[4] = buffer[len++];
        ptr[5] = buffer[len++];
        ptr[6] = buffer[len++];
        ptr[7] = buffer[len++];
        return *this;
    };
    BinaryStream & operator>>(std::string & s) {
        int lenght;
        (*this) >> lenght;
        s.clear();
        for (auto i = 0; i < lenght;i++)
            s.push_back((char)buffer[len++]);
        return *this;
    };

};


#endif
