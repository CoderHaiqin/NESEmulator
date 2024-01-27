#include "ppubus.h"
#include <assert.h>
#include <QDebug>

PPUBus::PPUBus() {}

void PPUBus::write(u16 addr, u8 value) {

    // qDebug() << "PPU write: " << Qt::hex << Qt::showbase << addr << " " << value << Qt::endl;
    if(addr < 0x1000) {
        assert(0);
        ppuMemory_.patternTable0->write(addr, value);
    } else if(addr < 0x2000) {
        assert(0);
        ppuMemory_.patternTable1->write(addr - 0x1000, value);
    } else if (addr < 0x2400) {

        ppuMemory_.nameTable0[addr - 0x2000] = value;
    } else if (addr < 0x2800) {
        ppuMemory_.nameTable1[addr - 0x2400] = value;
    } else if (addr < 0x2c00) {
        ppuMemory_.nameTable2[addr - 0x2800] = value;
    } else if (addr < 0x3000) {
        ppuMemory_.nameTable3[addr - 0x2c00] = value;
    } else if (addr < 0x3f00) {
        write(addr - 0x1000, value);
    } else {
        addr -= 0x3f00;
        addr = addr & 0x1f;
        if (!(addr & (u16)0x03)) {
            addr = addr & (~(0x10));
        }
        ppuMemory_.PaletteRAMIndex[addr] = value;

    }
}

u8 PPUBus::read(u16 addr) {
    if(addr < 0x1000) {
        return ppuMemory_.patternTable0->read(addr);
    } else if(addr < 0x2000) {
        return ppuMemory_.patternTable1->read(addr - 0x1000);
    } else if (addr < 0x2400) {
        return ppuMemory_.nameTable0[addr - 0x2000];
    } else if (addr < 0x2800) {
        return ppuMemory_.nameTable1[addr - 0x2400];
    } else if (addr < 0x2c00) {
        return ppuMemory_.nameTable2[addr - 0x2800];
    } else if (addr < 0x3000) {
        return ppuMemory_.nameTable3[addr - 0x2c00];
    } else if (addr < 0x3f00) {
        return read(addr - 0x1000);
    } else {
        addr -= 0x3f00;
        addr = addr & 0x1f;
        if (!(addr & (u16)0x03)) {
            addr = addr & (~(0xc));
        }
        return ppuMemory_.PaletteRAMIndex[addr];
    }

    return 0;
}

void PPUBus::bindPatternTable(int index, MemoryBlock* memoryBlock) {
    if(index == 0) {
        ppuMemory_.patternTable0 = memoryBlock;
    } else if (index == 1) {
        ppuMemory_.patternTable1 = memoryBlock;
    }
}
