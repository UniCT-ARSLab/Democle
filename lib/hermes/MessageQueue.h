#ifndef MULTI_ESP_NET_MESSAGE_QUEUE
#define MULTI_ESP_NET_MESSAGE_QUEUE
#include <Arduino.h>
#include <Message.h>
#include <map>
#include <queue>

class MessageQueue {
  public:
    std::map<String, std::queue<Message>> queue;
    // ~MessageQueue(); TODO
    void push(const String name, const Message *m);
    std::queue<Message> *get_messages(const String name);
};

#endif