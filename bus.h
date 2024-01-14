#ifndef BUS_H
#define BUS_H
#include "type.h"
#include "memory.h"

class Bus
{
private:
    Memory* memory;
public:
    Bus();

    void bindMemory(Memory* memory);

    uint8_t read(u16 address);
    void write(u16 address, u8 value);
};

#endif // BUS_H
