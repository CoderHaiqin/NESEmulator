#ifndef BUS_H
#define BUS_H
#include "type.h"
#include "ppu.h"
#include "ioregister.h"
#include "mapper.h"

class CPU;

class Bus
{
private:
    MemoryBlock* ram_;
    MemoryBlock* extendedRAM_;

    CPU* cpu_;
    PPU* ppu_;
    IORegister* ioRegister_;

    Mapper* mapper_;

    bool hasExtendedRAM_;

public:
    Bus();

    void bindRAM(MemoryBlock* ram);
    void bindExtendedRAM(MemoryBlock* extendedRAM);

    void bindPPU(PPU* ppu);
    void bindCPU(CPU* cpu);
    void bindIORegister(IORegister* ioRegister);

    void bindMapper(Mapper* mapper);

    u8 read(u16 address);
    void write(u16 address, u8 value);
};

#endif // BUS_H
