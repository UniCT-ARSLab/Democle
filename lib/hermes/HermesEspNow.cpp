#if defined(HAS_EMBEDDED) && defined(ESP32)

#include <HermesEspNow.h>

HermesEspNow *HermesEspNow::instance = nullptr;

HermesEspNow::HermesEspNow(const String name, uint8_t channel) : Hermes(name, ESP_NOW_ADDR_SIZE) {
    this->channel = channel;
}

bool HermesEspNow::init(const String name, uint8_t channel) {
    if (instance != nullptr) {
        return false;
    }

    instance = new HermesEspNow(name, channel);
    return true;
}

HermesEspNow *HermesEspNow::get_instance() { return instance; }

bool HermesEspNow::start() {
    WiFi.mode(WIFI_MODE_STA);

    if (esp_now_init()) {
        return false;
    }

    return true;
}

bool HermesEspNow::stop() {
    if (esp_now_deinit()) {
        return false;
    }

    WiFi.mode(WIFI_OFF);

    return true;
}

bool HermesEspNow::send_to_address(const uint8_t *addr_to, Message *m) {
    esp_now_peer_info_t peer;

    memset(&peer, 0, sizeof(peer));
    memcpy(peer.peer_addr, addr_to, ESP_NOW_ADDR_SIZE);
    peer.channel = this->channel;
    peer.encrypt = false;

    if (esp_now_add_peer(&peer)) {
        return false;
    }

    uint8_t size;
    uint8_t *to_send = m->serialize(&size);

    if (esp_now_send(addr_to, to_send, size)) {
        return false;
    }

    if (esp_now_del_peer(addr_to)) {
        return false;
    }

    delete (to_send);
    return true;
}

bool HermesEspNow::broadcast_send(Message *m) {
    uint8_t broadcast_addr[ESP_NOW_ADDR_SIZE] = {0xFF};
    return this->send_to_address(broadcast_addr, m);
}

bool HermesEspNow::register_recv_cb(const hermes_recv_cb_t cb) {
    return esp_now_register_recv_cb((esp_now_recv_cb_t)cb) == 0;
}
#endif
