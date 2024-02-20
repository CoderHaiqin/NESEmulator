#ifndef PPUBUS_H
#define PPUBUS_H
#include "mapper.h"

struct PPUMemory {
    u8 nameTable[0x4 * 0x400];

    u16 nameTable0 = 0;
    u16 nameTable1 = 0;
    u16 nameTable2 = 0;
    u16 nameTable3 = 0;


    u8 PaletteRAMIndex[0x20];

};

class PPUBus
{
private:
    Mapper* mapper_;
public:
    PPUMemory ppuMemory_;
    PPUBus();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);

    void bindMapper(Mapper* mapper);
    void updateMirror();
};

#endif // PPUBUS_H
