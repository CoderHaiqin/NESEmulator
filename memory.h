#ifndef MEMORY_H
#define MEMORY_H
#include "type.h"
#include "constant.h"

class Memory
{
private:
    u8* memory_;
public:
    Memory();
    ~Memory();

    u8 read(u16 address);
    void write(u16 address, u8 value);
};

#endif // MEMORY_H
