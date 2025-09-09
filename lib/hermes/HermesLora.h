#ifndef MULTI_ESP_NET_LORA_H
#define MULTI_ESP_NET_LORA_H

#if defined(HAS_EMBEDDED) && defined(ESP32)

#define LM_GOD_MODE 1
#include <Hermes.h>
#include <LoraMesher.h>
#ifdef ESP32
#include <WiFi.h>
#include <esp_wifi.h>
#endif
#include <SPI.h>
#define LORA_MESH_ADDR_SIZE 2

class HermesLora : public Hermes {
  protected:
    static HermesLora *instance;
    HermesLora(const String name, const uint8_t lora_cs, const uint8_t lora_irq,
               const uint8_t lora_rst, const uint8_t lora_io1);
    HermesLora(const String name, 
               const uint8_t lora_cs, const uint8_t lora_irq, 
               const uint8_t lora_rst, const uint8_t lora_io1,
               const float freq, const int8_t power,
               int8_t spi_miso, int8_t spi_mosi, int8_t spi_clk);

  public:
    // MultiEspNetLora should not be cloneable.
    HermesLora(HermesLora &other) = delete;

    // MultiEspNetLora should not be assignable.
    void operator=(const HermesLora &) = delete;

    static bool init(const String name, const uint8_t lora_cs,
                     const uint8_t lora_irq, const uint8_t lora_rst,
                     const uint8_t lora_io1);
    static bool init(const String name, const uint8_t lora_cs,
                     const uint8_t lora_irq, const uint8_t lora_rst,
                     const uint8_t lora_io1, const float freq, const int8_t power,
                     int8_t spi_miso, int8_t spi_mosi, int8_t spi_clk);
    static HermesLora *get_instance();
    bool start() override;
    bool stop() override;
    bool send_to_address(const uint8_t *addr_to, Message *m) override;
    bool broadcast_send(Message *m) override;
    bool register_recv_cb(const hermes_recv_cb_t cb) override;
    SPIClass * radios_spi;
};

#endif
#endif
