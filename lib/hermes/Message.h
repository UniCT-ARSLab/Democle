#ifndef MULTI_ESP_NET_MESSAGE_H
#define MULTI_ESP_NET_MESSAGE_H
#include <Arduino.h>

typedef enum {
    WHOIS_REQUEST_MESSAGE,
    WHOIS_ANSWER_MESSAGE,
    DEFAULT_MESSAGE
} MessageType;

#define MESSAGE_TYPE_SIZE sizeof(MessageType)

typedef struct {
    String name;

} whois_request_payload_t;

typedef struct {

} whois_answer_payload_t;

class Message {
  public:
    MessageType type;
    uint8_t *buffer;
    uint8_t size;

    Message(MessageType type, const uint8_t *buffer, uint8_t size);
    Message(const uint8_t *from_buffer, uint8_t size);
    Message(const Message& m);
    ~Message();

    uint8_t *serialize(uint8_t *size);
};

#endif