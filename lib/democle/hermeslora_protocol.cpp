/*
*   hermeslora_protocol.cpp
*/

#if defined(HAS_EMBEDDED) && defined(ESP32)

#include "democle.h"
#include <Arduino.h>
#include <HermesLora.h>
#include <LoraMesher.h>


HermesLora * gmnet; //global ref to hermes mesh (LoRa)

void hermeslora_thread_start(HermesLoraProtocol * p)
{
    p->run();
}

HermesLoraProtocol::HermesLoraProtocol
(
    const std::string &device_name, 
    int loraCS, int loraIRQ, int loraRST, int loraIO1
) : AbstractProtocol(), 
    DEVICE_NAME(device_name), 
    LORA_CS(loraCS), 
    LORA_IRQ(loraIRQ), 
    LORA_RST(loraRST), 
    LORA_IO1(loraIO1) 
{
    run();
}


HermesLoraProtocol::HermesLoraProtocol
(
    const std::string &device_name, 
    int loraCS, int loraIRQ, int loraRST, int loraIO1, float freq, int power, int spi_miso, int spi_mosi, int spi_clk
) : AbstractProtocol(), 
    DEVICE_NAME(device_name), 
    LORA_CS(loraCS), 
    LORA_IRQ(loraIRQ), 
    LORA_RST(loraRST), 
    LORA_IO1(loraIO1),
    FREQ(freq),
    POWER(power),
    SPI_MISO(spi_miso),
    SPI_MOSI(spi_mosi),
    SPI_CLK(spi_clk) 
{
    run();
}

void lora_cb(const uint8_t *addr, const uint8_t *payload, int len){
    Message m(payload, len);

    switch (m.type) {
        case WHOIS_REQUEST_MESSAGE:
        {
            gmnet->handle_whois_request(addr, &m);
            return;
        }
        case WHOIS_ANSWER_MESSAGE:
        {
            gmnet->handle_whois_answer(addr, &m);
            return;
        }
        case DEFAULT_MESSAGE:
        {
            // Serial.println("Packet inbound:");
                
            DEMOCLEPacket p((uint8_t *)m.buffer, m.size);
            std::string sender_agent_name, dest_agent_name;
            term_vector t;
            AtomicFormula af(t);
            p.get(sender_agent_name);
            p.get(dest_agent_name);
            p.get(af);
            std::string full_sender_agent_name = sender_agent_name;
            if (sender_agent_name.rfind("hermeslora://", 0) != 0) {
                full_sender_agent_name = "hermeslora://" + sender_agent_name;
            }
            // Serial.printf("sender: %s, dest: %s, af: %s\n",
            //     full_sender_agent_name.c_str(), dest_agent_name.c_str(), af.get_name().c_str());
            DEMOCLE::put_message_in_queue(dest_agent_name, full_sender_agent_name, af);
            break; 
        }
        default:
            return;
    }
}

void HermesLoraProtocol::run()
{
    if(SPI_MISO != -1 && SPI_MOSI != -1 && SPI_CLK != -1) {
        HermesLora::init(DEVICE_NAME.c_str(), LORA_CS, LORA_IRQ, LORA_RST, LORA_IO1, FREQ, POWER, SPI_MISO, SPI_MOSI, SPI_CLK);
    } else {
        // Default frequency and power
        HermesLora::init(DEVICE_NAME.c_str(), LORA_CS, LORA_IRQ, LORA_RST, LORA_IO1);
    }
    _mnet = HermesLora::get_instance();
    gmnet = _mnet;
    if(!(_mnet->start())) {return;}
    if(!_mnet->register_recv_cb(lora_cb)){return;}
}

void HermesLoraProtocol::send_message(Agent * sender, url & destination, AtomicFormula & a)
{
    // Serial.printf("Sender name: %s\n", sender->get_name().c_str());
    // Serial.printf("LoRa target name: %s\n", destination.host.c_str());
    // Serial.printf("Target agent: %s\n", destination.path.c_str());
    // Serial.printf("Atomic formula: %s\n", a.get_name().c_str());
    // Serial.printf("Con parametro: %s\n", string(a.get_terms()[0].value()).c_str());
    
    DEMOCLEPacket p;
    std::string source_url = //syntax: "protocol://LoRaName/AgentName"
        DEVICE_NAME + "/" + sender->get_name();
    p.put(source_url.c_str());
    p.put(destination.path.c_str());
    p.put(a);
    
    Message message(DEFAULT_MESSAGE, (uint8_t*)p.data(), p.size());
    
    // Serial.printf("\nSEND MESSAGE SIZE: %d\n", p.size());
    
    _mnet->send(destination.host.c_str(), &message);
}
#endif
