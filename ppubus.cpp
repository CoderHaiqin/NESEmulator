#include "ppubus.h"

PPUBus::PPUBus() {}

void PPUBus::write(u16 addr, u8 value) {
    if(addr < 0x1000) {
        ppuMemory_.patternTable0->write(addr, value);
    } else if(addr < 0x2000) {
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
        ppuMemory_.PaletteRAMIndex[(addr - 0x3f00) % 0x20] = value;
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
        return ppuMemory_.PaletteRAMIndex[(addr - 0x3f00) % 0x20];
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
