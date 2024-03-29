#ifndef MAPPER_H
#define MAPPER_H
#include "type.h"
#include "rom.h"

class PPUBus;

class Mapper
{
public:
    ROM* rom;
    PPUBus* ppuBus;
    virtual u8 readPRG(u16 address) = 0;
    virtual void writePRG(u16 address, u8 value) = 0;
    virtual u8 readCHR(u16 address) = 0;
    virtual void writeCHR(u16 address, u8 value) = 0;

    virtual void bindPPUBus(PPUBus* ppu);
    virtual u8 getNameTableMirror();
    Mapper();

    static Mapper* createFromROM(ROM* rom);
};

#endif // MAPPER_H
