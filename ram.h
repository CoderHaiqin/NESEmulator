#ifndef RAM_H
#define RAM_H
#include "type.h"
#include "constant.h"

class RAM
{
private:
    u8 ram_[Constant::RAMSize];
public:
    RAM();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);
};

#endif // RAM_H
