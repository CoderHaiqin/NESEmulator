#ifndef BUS_H
#define BUS_H
#include "type.h"
#include "memoryblock.h"
#include "ppu.h"
#include "ioregister.h"

class Bus
{
private:
    MemoryBlock* ram_;
    MemoryBlock* prg_1_;
    MemoryBlock* prg_2_;
    PPU* ppu_;
    IORegister* ioRegister_;

public:
    Bus();

    void bindRAM(MemoryBlock* ram);
    void bindPRG_1(MemoryBlock* prg);
    void bindPRG_2(MemoryBlock* prg);
    void bindPPU(PPU* ppu);
    void bindIORegister(IORegister* ioRegister);

    u8 read(u16 address);
    void write(u16 address, u8 value);
};

#endif // BUS_H
