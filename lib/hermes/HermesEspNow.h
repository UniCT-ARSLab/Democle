#ifndef HERMES_ESP_NOW_H
#define HERMES_ESP_NOW_H
#include "Hermes.h"

#if defined(HAS_EMBEDDED) && defined(ESP32)

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

#define ESP_NOW_ADDR_SIZE 6

class HermesEspNow : public Hermes {
  protected:
    uint8_t channel;
    static HermesEspNow *instance;
    HermesEspNow(const String name, uint8_t channel);

  public:
    static hermes_recv_cb_t recv_cb;

    // MultiEspNetEspNow should not be cloneable.
    HermesEspNow(HermesEspNow &other) = delete;

    // MultiEspNetEspNow should not be assignable.
    void operator=(const HermesEspNow &) = delete;

    static bool init(const String name, uint8_t channel);
    static HermesEspNow *get_instance();
    bool start() override;
    bool stop() override;
    bool send_to_address(const uint8_t *addr_to, Message *m) override;
    bool broadcast_send(Message *m) override;
    bool register_recv_cb(const hermes_recv_cb_t cb) override;
};

#endif
#endif