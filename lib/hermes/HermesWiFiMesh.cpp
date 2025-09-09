#if defined(HAS_EMBEDDED) && defined(ESP32)


#include <HermesWiFiMesh.h>

HermesWiFiMesh *HermesWiFiMesh::instance = nullptr;

HermesWiFiMesh::HermesWiFiMesh(const String name, const String net_id,
                               const uint8_t channel, const bool is_root,
                               const uint8_t max_connections)
    : Hermes(name, WIFI_MESH_ADDR_SIZE) {
    this->is_root = is_root;

    mesh_cfg_t def_config = MESH_INIT_CONFIG_DEFAULT();

    memcpy(&this->mesh_cfg, &def_config, sizeof(mesh_cfg_t));

    memcpy((uint8_t *)&this->mesh_cfg.mesh_id, (uint8_t *)net_id.c_str(),
           net_id.length());

    this->mesh_cfg.channel = channel;
    this->mesh_cfg.allow_channel_switch = false;
    this->mesh_cfg.mesh_ap.max_connection = max_connections;
    this->mesh_cfg.mesh_ap.nonmesh_max_connection = 0;
    this->mesh_cfg.crypto_funcs = NULL;
}

bool HermesWiFiMesh::init(const String name, const String net_id,
                          const uint8_t channel, const bool is_root,
                          const uint8_t max_connections) {
    if (instance != nullptr) {
        return false;
    }

    instance =
        new HermesWiFiMesh(name, net_id, channel, is_root, max_connections);
    return true;
}

HermesWiFiMesh *HermesWiFiMesh::get_instance() { return instance; }

bool HermesWiFiMesh::start() {
    if (nvs_flash_init()) {
        return false;
    }

    if (esp_netif_init()) {
        return false;
    }

    if (esp_event_loop_create_default()) {
        return false;
    }

    if (esp_netif_create_default_wifi_mesh_netifs(NULL, NULL)) {
        return false;
    }

    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();

    if (esp_wifi_init(&config)) {
        return false;
    }

    if (esp_wifi_set_storage(WIFI_STORAGE_FLASH)) {
        return false;
    }

    if (esp_wifi_set_ps(WIFI_PS_NONE)) {
        return false;
    }

    if (esp_mesh_disable_ps()) {
        return false;
    }

    if (esp_wifi_start()) {
        return false;
    }

    if (esp_mesh_init()) {
        return false;
    }

    if (esp_mesh_fix_root(true)) {
        return false;
    }

    if (this->is_root) {
        if (esp_mesh_set_type(MESH_ROOT)) {
            return false;
        }
    }

    esp_mesh_set_config(&this->mesh_cfg);
    esp_err_t err;

    while ((err = esp_mesh_start())) {
        Serial.print("Error: esp_mesh_start: ");
        Serial.printf("%s\n", esp_err_to_name(err));
        delay(1000);
    }

    return true;
}

bool HermesWiFiMesh::stop() {
    if (esp_mesh_stop()) {
        return false;
    }

    if (esp_wifi_stop()) {
        return false;
    }

    return true;
}

bool HermesWiFiMesh::send_to_address(const uint8_t *addr_to, Message *m) {
    mesh_addr_t addr;
    mesh_data_t data;

    memcpy(&addr.addr, addr_to, WIFI_MESH_ADDR_SIZE);

    uint8_t size;
    uint8_t *to_send = m->serialize(&size);
    data.data = to_send;
    data.size = size;
    data.tos = MESH_TOS_P2P;
    data.proto = MESH_PROTO_BIN;

    esp_err_t res = esp_mesh_send(&addr, &data, MESH_DATA_P2P, NULL, 0);

    delete (to_send);
    return res == 0;
}

bool HermesWiFiMesh::broadcast_send(Message *m) {
    uint8_t broadcast_addr[WIFI_MESH_ADDR_SIZE] = {0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF};
    return this->send_to_address(broadcast_addr, m);
}

void wifi_mesh_recv_task(void *prms) {
    esp_err_t err;
    hermes_recv_cb_t cb = (hermes_recv_cb_t)prms;

    uint8_t *buffer =
        (uint8_t *)malloc(WIFI_MESH_RX_SIZE); // Memory leak: needs fix
    // uint8_t buffer[WIFI_MESH_RX_SIZE];
    mesh_addr_t *addr = (mesh_addr_t *)malloc(sizeof(mesh_addr_t));
    mesh_data_t *data = (mesh_data_t *)malloc(sizeof(mesh_data_t));
    data->data = buffer;
    data->size = WIFI_MESH_RX_SIZE;

    while (1) {
        if ((err = esp_mesh_recv(addr, data, portMAX_DELAY, NULL, NULL, 0)) !=
            ESP_OK) {
            printf("Error: esp_mesh_recv: %s\n", esp_err_to_name(err));
            continue;
        } else {
            cb(addr->addr, data->data, data->size);
        }
    }
}

// TODO: vTaskDelete in stop function
bool HermesWiFiMesh::register_recv_cb(const hermes_recv_cb_t cb) {
    if (xTaskCreate(wifi_mesh_recv_task, "hermes_recv_task", 2000, (void *)cb,
                    configMAX_PRIORITIES - 1, NULL) <= 0) {
        return false;
    }

    return true;
}
#endif
