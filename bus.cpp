#include "bus.h"
#include <iostream>

Bus::Bus() : ram_(nullptr) {}

void Bus::bindRAM(MemoryBlock *ram) {
    ram_ = ram;
}

void Bus::bindPRG(MemoryBlock *prg) {
    prg_ = prg;
}

uint8_t Bus::read(u16 address) {
    u16 key = address >> 13;
    if(key == 0) {
        ram_->read(address & 0x7ff);
    }
    else {
        prg_->read(address % prg_->length_);
    }

    return 0;
}

void Bus::write(u16 address, u8 value) {
    u16 key = address >> 13;
    if(key == 0) {
        ram_->write(address & 0x7ff, value);
    }
    else {
        prg_->write(address % prg_->length_, value);
    }
}
