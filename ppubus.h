#ifndef PPUBUS_H
#define PPUBUS_H
#include "mapper.h"

struct PPUMemory {
    u8 nameTable0[0x400];
    u8 nameTable1[0x400];
    u8 nameTable2[0x400];
    u8 nameTable3[0x400];

    u8 PaletteRAMIndex[0x20];

};

class PPUBus
{
private:
    Mapper* mapper_;
    PPUMemory ppuMemory_;
public:
    PPUBus();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);

    void bindMapper(Mapper* mapper);
};

#endif // PPUBUS_H
