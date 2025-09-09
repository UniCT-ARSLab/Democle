
#if defined(HAS_EMBEDDED) && defined(ESP32)

#include <HermesLora.h>

#include <LoraMesher.h>
#define MAXPACKETSIZE 90

HermesLora *HermesLora::instance = nullptr;

HermesLora::HermesLora(const String name, const uint8_t lora_cs,
                       const uint8_t lora_irq, const uint8_t lora_rst,
                       const uint8_t lora_io1)
    : Hermes(name, LORA_MESH_ADDR_SIZE) {
    LoraMesher::LoraMesherConfig config;
    config.loraCs = lora_cs;
    config.loraIrq = lora_irq;
    config.loraRst = lora_rst;
    config.loraIo1 = lora_io1;

    LoraMesher::getInstance().begin(config);
}

HermesLora::HermesLora(const String name, const uint8_t lora_cs,
                       const uint8_t lora_irq, const uint8_t lora_rst,
                       const uint8_t lora_io1, const float freq, const int8_t power,
                       int8_t spi_miso, int8_t spi_mosi, int8_t spi_clk)
    : Hermes(name, LORA_MESH_ADDR_SIZE) {
    LoraMesher::LoraMesherConfig config;
    config.loraCs = lora_cs;
    config.loraIrq = lora_irq;
    config.loraRst = lora_rst;
    config.loraIo1 = lora_io1;
    config.freq = freq;
    config.power = power;

    radios_spi = new SPIClass(HSPI);
    radios_spi->setBitOrder(MSBFIRST);
    radios_spi->setDataMode(SPI_MODE0);
    radios_spi->begin(spi_clk, spi_miso, spi_mosi);
    config.spi = radios_spi;

    LoraMesher::getInstance().begin(config);
}

bool HermesLora::init(const String name, const uint8_t lora_cs,
                      const uint8_t lora_irq, const uint8_t lora_rst,
                      const uint8_t lora_io1) {
    if (instance != nullptr) {
        return false;
    }

    instance = new HermesLora(name, lora_cs, lora_irq, lora_rst, lora_io1);
    return true;
}

bool HermesLora::init(const String name, const uint8_t lora_cs,
                      const uint8_t lora_irq, const uint8_t lora_rst,
                      const uint8_t lora_io1, const float freq, const int8_t power,
                      int8_t spi_miso, int8_t spi_mosi, int8_t spi_clk) {
    if (instance != nullptr) {
        return false;
    }

    instance = new HermesLora(name, lora_cs, lora_irq, lora_rst, lora_io1,
                              freq, power, spi_miso, spi_mosi, spi_clk);
    return true;
}

HermesLora *HermesLora::get_instance() { return instance; }

bool HermesLora::start() {
    LoraMesher::getInstance().start();
    return true;
}

// TO FIX
bool HermesLora::stop() {
    LoraMesher::getInstance().standby();
    return true;
}

void sendHelloPacketNow() {
    size_t maxNodesPerPacket =
        (MAXPACKETSIZE - sizeof(RoutePacket)) / sizeof(NetworkNode);

    LoraMesher::getInstance().incSentHelloPackets();

    NetworkNode *nodes = RoutingTableService::getAllNetworkNodes();
    size_t numOfNodes = RoutingTableService::routingTableSize();
    size_t numPackets =
        (numOfNodes + maxNodesPerPacket - 1) / maxNodesPerPacket;
    numPackets = (numPackets == 0) ? 1 : numPackets;

    for (size_t i = 0; i < numPackets; ++i) {
        size_t startIndex = i * maxNodesPerPacket;
        size_t endIndex = startIndex + maxNodesPerPacket;
        if (endIndex > numOfNodes) {
            endIndex = numOfNodes;
        }

        size_t nodesInThisPacket = endIndex - startIndex;

        // Create and send the packet
        RoutePacket *tx = PacketService::createRoutingPacket(
            LoraMesher::getInstance().getLocalAddress(), &nodes[startIndex],
            nodesInThisPacket, RoleService::getRole());

        LoraMesher::getInstance().setPackedForSend(
            reinterpret_cast<Packet<uint8_t> *>(tx), DEFAULT_PRIORITY + 1);
    }

    // Delete the nodes array
    if (numOfNodes > 0)
        delete[] nodes;
}

bool HermesLora::send_to_address(const uint8_t *addr_to, Message *m) {
    uint16_t addr = addr_to[0] | (addr_to[1] << 8);
    uint8_t size;
    uint8_t *to_send = m->serialize(&size);
    LoraMesher &radio = LoraMesher::getInstance();

    if (m->type == WHOIS_REQUEST_MESSAGE || m->type == WHOIS_ANSWER_MESSAGE) {
        sendHelloPacketNow();
    }

    radio.createPacketAndSend(addr, to_send, size);

    delete (to_send);
    return true;
}

bool HermesLora::broadcast_send(Message *m) {
    uint8_t broadcast_addr[2] = {0xFF, 0xFF};
    return this->send_to_address(broadcast_addr, m);
}

void process_received_packets(void *callback) {
    const hermes_recv_cb_t cb = (const hermes_recv_cb_t)callback;
    LoraMesher &radio = LoraMesher::getInstance();

    while (1) {
        /* Wait for the notification of processReceivedPackets and enter
         * blocking */
        ulTaskNotifyTake(pdPASS, portMAX_DELAY);

        // Iterate through all the packets inside the Received User Packets
        // Queue
        while (radio.getReceivedQueueSize() > 0) {
            // Get the first element inside the Received User Packets Queue
            AppPacket<uint8_t> *packet = radio.getNextAppPacket<uint8_t>();

            uint8_t src[LORA_MESH_ADDR_SIZE] = {packet->src & 0xff,
                                                packet->src >> 8};

            cb(src, packet->payload, packet->payloadSize);

            // Delete the packet when used. It is very important to call
            // this function to release the memory of the packet.
            radio.deletePacket(packet);
        }
    }
}

bool HermesLora::register_recv_cb(const hermes_recv_cb_t cb) {
    TaskHandle_t cb_h = NULL;
    if (xTaskCreate(process_received_packets, "hermes_recv_task", 4096,
                    (void *)cb, 2, &cb_h) <= 0) {
        return false;
    }

    if (cb_h != NULL) {
        LoraMesher::getInstance().setReceiveAppDataTaskHandle(cb_h);
        return true;
    }

    return false;
}
#endif
