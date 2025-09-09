#include <NameService.h>

NameService::NameService(uint8_t _address_size) : address_size(_address_size) {}

void NameService::add(const String& name, const uint8_t* addr) {
    std::vector<uint8_t> copy(addr, addr + address_size);
    address_table[name] = std::move(copy);
}

bool NameService::exists(const String& name) const {
    return address_table.count(name) > 0;
}

const uint8_t* NameService::get(const String& name) const {
    auto it = address_table.find(name);
    if (it != address_table.end()) {
        return it->second.data();
    }
    return nullptr;
}

void NameService::remove(const String& name) {
    address_table.erase(name);
}
