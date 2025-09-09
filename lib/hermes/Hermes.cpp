#include <Hermes.h>

Hermes::Hermes(String name, uint8_t addr_size) : name_service(addr_size){ 
    this->name = name;
}

bool Hermes::send_whois_request(const String name) {
    Message message(WHOIS_REQUEST_MESSAGE, (const uint8_t *)name.c_str(),
                    name.length() + 1);
    return this->broadcast_send(&message);
}

bool Hermes::handle_whois_request(const uint8_t *from, Message *m) {
    String rname = (const char *)m->buffer;
    if (this->name == rname) {
        Message whois_answer(WHOIS_ANSWER_MESSAGE, m->buffer, m->size);
        this->send_to_address(from, &whois_answer);
    }

    return true;
}

bool Hermes::handle_whois_answer(const uint8_t *from, Message *m) {
    String name = (const char *)m->buffer;
    this->name_service.add(name, from);

    const uint8_t *address = this->name_service.get(name);
    if (address == nullptr) {
        return false;
    }

    std::queue<Message> *q = this->message_queue.get_messages(name);

    if (q != nullptr) {
        while (!q->empty()) {
            Message m = q->front();
            this->send(name, &m);
            q->pop();
        }
    }

    return true;
}

bool Hermes::send(const String name, Message *m) {
    if (!this->name_service.exists(name)) {
        this->message_queue.push(name, m);
        this->send_whois_request(name);
        return true;
    }
    const uint8_t *address = this->name_service.get(name);
    return this->send_to_address(address, m);
}