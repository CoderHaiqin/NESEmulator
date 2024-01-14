#ifndef REGISTERS_H
#define REGISTERS_H

#include "type.h"

class Registers
{
public:
    u8 A;
    u8 X;
    u8 Y;
    u16 PC;
    u8 SP;
    u8 P;
    Registers();
};

#endif // REGISTERS_H
