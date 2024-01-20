#ifndef BUS_H
#define BUS_H
#include "type.h"
#include "RAM.h"

class Bus
{
private:
    RAM* ram_;
public:
    Bus();

    void bindRAM(RAM* ram);

    u8 read(u16 address);
    void write(u16 address, u8 value);
};

#endif // BUS_H
