#include <MessageQueue.h>

void MessageQueue::push(const String name, const Message *m) {
    this->queue[name].push(*m);
}

std::queue<Message> *MessageQueue::get_messages(const String name) {
    return &(this->queue[name]);
}