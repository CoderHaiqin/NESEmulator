#ifndef RAM_H
#define RAM_H
#include "type.h"


class MemoryBlock
{
public:
    u8* m_;
    u16 length_;

    MemoryBlock(u16 length);
    ~MemoryBlock();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);
};

#endif // RAM_H
