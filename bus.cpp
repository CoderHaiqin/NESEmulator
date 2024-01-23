#include "bus.h"
#include <iostream>

Bus::Bus() : ram_(nullptr) {}

void Bus::bindRAM(MemoryBlock *ram) {
    ram_ = ram;
}

void Bus::bindPRG_1(MemoryBlock *prg) {
    prg_1_ = prg;
}

void Bus::bindPRG_2(MemoryBlock *prg) {
    prg_2_ = prg;
}

void Bus::bindPPU(PPU* ppu) {
    ppu_ = ppu;
}

uint8_t Bus::read(u16 address) {
    if(address < 0x2000) {
        return ram_->read(address & 0x7ff);
    } else if (address < 0x4000) {
        return ppu_->read(address - 0x2000);
    } else if(address < 0x4020) {

    } else if(address < 0x6000) {

    } else if(address < 0x8000) {

    } else if(address < 0xc000) {
        return prg_1_->read(address - 0x8000);
    } else {
        return prg_2_->read(address - 0xc000);
    }

    return 0;
}

void Bus::write(u16 address, u8 value) {
    if(address < 0x2000) {
        ram_->write(address & 0x7ff, value);
    } else if (address < 0x4000) {
        ppu_->write(address - 0x2000, value);
    } else if(address < 0x4020) {

    } else if(address < 0x6000) {

    } else if(address < 0x8000) {

    } else if(address < 0xc000) {
        prg_1_->write(address - 0x8000, value);
    } else {
        prg_2_->write(address - 0xc000, value);
    }
}
