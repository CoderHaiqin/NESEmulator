#include "bus.h"
#include <QDebug>

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

u8 Bus::read(u16 address) {
    //qDebug() << "read: " << Qt::hex << Qt::showbase << address << Qt::endl;
    u8 result = 0;
    if(address < 0x2000) {
        result = ram_->read(address & 0x7ff);
    } else if (address < 0x4000) {
        result = ppu_->read(address - 0x2000);
    } else if(address < 0x4020) {
        result = ioRegister_->read(address - 0x4000);
    } else if(address < 0x6000) {

    } else if(address < 0x8000) {

    } else if(address < 0xc000) {
        result = prg_1_->read(address - 0x8000);
    } else {
        result = prg_2_->read(address - 0xc000);
    }

    return result;
}

void Bus::write(u16 address, u8 value) {

    //qDebug() << "write: " << Qt::hex << Qt::showbase << value << " to "<< Qt::hex << Qt::showbase << address<< Qt::endl;
    if(address < 0x2000) {
        ram_->write(address & 0x7ff, value);
    } else if (address < 0x4000) {
        ppu_->write(address - 0x2000, value);
    } else if(address < 0x4020) {
        ioRegister_->write(address - 0x4000, value);
    } else if(address < 0x6000) {

    } else if(address < 0x8000) {

    } else if(address < 0xc000) {
        prg_1_->write(address - 0x8000, value);
    } else {
        prg_2_->write(address - 0xc000, value);
    }
}

void Bus::bindIORegister(IORegister* ioRegister) {
    ioRegister_ = ioRegister;
}
