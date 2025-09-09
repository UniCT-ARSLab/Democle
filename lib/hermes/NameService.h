#ifndef MULTI_ESP_NET_NAME_SERVICE_H
#define MULTI_ESP_NET_NAME_SERVICE_H
#include <Arduino.h>
#include <Message.h>
#include <map>
#include <vector>

class NameService {
  public:
    NameService(uint8_t _address_size);
    ~NameService() = default;

    void add(const String& name, const uint8_t* addr);
    bool exists(const String& name) const;
    const uint8_t* get(const String& name) const;
    void remove(const String& name);

    // TODO : make private and add setter
    std::map<String, std::vector<uint8_t>> address_table;
  private:
    uint8_t address_size;
};

#endif