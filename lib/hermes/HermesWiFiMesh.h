#ifndef HERMES_WIFI_MESH_H
#define HERMES_WIFI_MESH_H

#if defined(HAS_EMBEDDED) && defined(ESP32)

#include <Hermes.h>
#include <Message.h>
#include <esp_mesh.h>
#include <esp_wifi.h>
#include <nvs_flash.h>
#define WIFI_MESH_ADDR_SIZE 6
#define WIFI_MESH_RX_SIZE (1500)

class HermesWiFiMesh : public Hermes {
  protected:
    bool is_root;
    mesh_cfg_t mesh_cfg;
    static HermesWiFiMesh *instance;

    HermesWiFiMesh(const String name, const String net_id,
                   const uint8_t channel, const bool is_root,
                   const uint8_t max_connections);

  public:
    // MultiEspNetWiFiMesh should not be cloneable.
    HermesWiFiMesh(HermesWiFiMesh &other) = delete;

    // MultiEspNetWiFiMesh should not be assignable.
    void operator=(const HermesWiFiMesh &) = delete;

    static bool init(const String name, const String net_id,
                     const uint8_t channel, const bool is_root,
                     const uint8_t max_connections);
    static HermesWiFiMesh *get_instance();
    bool start() override;
    bool stop() override;
    bool send_to_address(const uint8_t *addr_to, Message *m) override;
    bool broadcast_send(Message *m) override;
    bool register_recv_cb(const hermes_recv_cb_t cb) override;
};

#endif
#endif
