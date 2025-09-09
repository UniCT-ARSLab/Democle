#include <Message.h>

Message::Message(MessageType type, const uint8_t *buffer, uint8_t size) {
    this->type = type;
    this->buffer = (uint8_t *)malloc(size);
    memcpy(this->buffer, buffer, size);
    this->size = size;
}

Message::Message(const uint8_t *from_buffer, uint8_t size) {
    this->size = size - MESSAGE_TYPE_SIZE;
    this->buffer = (uint8_t *)malloc(this->size);

    memcpy(&this->type, from_buffer, MESSAGE_TYPE_SIZE);
    memcpy(this->buffer, from_buffer + MESSAGE_TYPE_SIZE, this->size);
}

Message::Message(const Message &m) {
    this->type = m.type;
    this->size = m.size;
    this->buffer = (uint8_t *)malloc(m.size);
    memcpy(this->buffer, m.buffer, m.size);
}

Message::~Message() { delete (this->buffer); }

uint8_t *Message::serialize(uint8_t *size) {
    uint8_t *out = (uint8_t *)malloc(this->size + MESSAGE_TYPE_SIZE);

    memcpy(out, &this->type, MESSAGE_TYPE_SIZE);
    memcpy(out + MESSAGE_TYPE_SIZE, this->buffer, this->size);

    *size = this->size + MESSAGE_TYPE_SIZE;

    return out;
}