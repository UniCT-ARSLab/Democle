/*
 * protocol.h
 */

#ifndef __PROTOCOL_H
#define __PROTOCOL_H

#include "democle.h"
#include "HermesLora.h"

class AbstractProtocol {
 public:
    AbstractProtocol() { };
    virtual void send_message(Agent * sender, url & destination, AtomicFormula & a) = 0;
};

#if defined(HAS_EMBEDDED) && defined(ESP32)
class HermesLoraProtocol : public AbstractProtocol {
    std::string DEVICE_NAME;
    string dest;
    std::thread * hl_thread;
    int LORA_CS, LORA_IRQ, LORA_RST, LORA_IO1;
    float FREQ = 868.0; // Default frequency
    int POWER = 2; // Default power in dBm
    int SPI_MISO = -1, SPI_MOSI = -1, SPI_CLK = -1; // Default SPI pins
    HermesLora *_mnet;
public:
    HermesLoraProtocol(const std::string &, int, int, int, int);
    HermesLoraProtocol(const std::string &, int, int, int, int, float, int, int, int, int);
    virtual void send_message(Agent * sender, url & destination, AtomicFormula & a) override;
    void run();
};
#endif

#ifndef HAS_EMBEDDED
class TCPProtocol : public AbstractProtocol {
    int port_number;
    string host;
    thread * tcp_thread;
    int server_fd;
 public:
    TCPProtocol(string _host,int port_num);
    virtual void send_message(Agent * sender, url & destination, AtomicFormula & a) override;
    void run();
};

#endif
#endif
