#ifndef BUS_H
#define BUS_H
#include "type.h"
#include "memoryblock.h"

class Bus
{
private:
    MemoryBlock* ram_;
    MemoryBlock* prg_;
public:
    Bus();

    void bindRAM(MemoryBlock* ram);
    void bindPRG(MemoryBlock* prg);

    u8 read(u16 address);
    void write(u16 address, u8 value);
};

#endif // BUS_H
