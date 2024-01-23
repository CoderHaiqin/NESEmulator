#ifndef PPUBUS_H
#define PPUBUS_H
#include "type.h"
#include "memoryblock.h"


struct PPUMemory {
    MemoryBlock* patternTable0;
    MemoryBlock* patternTable1;

    u8 nameTable0[0x400];
    u8 nameTable1[0x400];
    u8 nameTable2[0x400];
    u8 nameTable3[0x400];

    u8 PaletteRAMIndex[0x20];

};

class PPUBus
{
private:
    PPUMemory ppuMemory_;
public:
    PPUBus();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);

    void bindPatternTable(int index, MemoryBlock* memoryBlock);
};

#endif // PPUBUS_H
