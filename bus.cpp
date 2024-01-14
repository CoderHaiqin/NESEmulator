#include "bus.h"

Bus::Bus() : memory(nullptr) {}

void Bus::bindMemory(Memory *memory) {
    this->memory = memory;
}


uint8_t Bus::read(u16 address) {
    return this->memory->read(address);
}

void Bus::write(u16 address, u8 value) {
    this->memory->write(address, value);
}
