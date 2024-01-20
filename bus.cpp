#include "bus.h"

Bus::Bus() : ram_(nullptr) {}

void Bus::bindRAM(RAM *ram) {
    ram_ = ram;
}

uint8_t Bus::read(u16 address) {
    return ram_->read(address);
}

void Bus::write(u16 address, u8 value) {
    ram_->write(address, value);
}
