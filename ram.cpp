#include "ram.h"

RAM::RAM() {}

void RAM::write(u16 addr, u8 value) {
    if(addr < Constant::RAMSize) {
        ram_[addr] = value;
    }
    else {
        return;
    }
}

u8 RAM::read(u16 addr) {
    if(addr < Constant::RAMSize) {
        return ram_[addr];
    }

    return 0;
}
