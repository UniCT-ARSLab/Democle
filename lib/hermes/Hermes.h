#ifndef HERMES_H
#define HERMES_H
#include <Arduino.h>
#include <Message.h>
#include <MessageQueue.h>
#include <NameService.h>

typedef void (*hermes_recv_cb_t)(const uint8_t *addr, const uint8_t *payload,
                                 int len);

class Hermes {
  public:
    String name;
    NameService name_service;
    MessageQueue message_queue;

    Hermes(const String name, uint8_t addr_size);
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool send_to_address(const uint8_t *addr_to, Message *m) = 0;
    virtual bool broadcast_send(Message *m) = 0;
    virtual bool register_recv_cb(const hermes_recv_cb_t cb) = 0;

    bool send(const String name, Message *m);
    bool send_whois_request(const String name);
    bool handle_whois_request(const uint8_t *from, Message *m);
    bool handle_whois_answer(const uint8_t *from, Message *m);
};

#endif
