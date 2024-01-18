#ifndef REGISTERS_H
#define REGISTERS_H

#include "type.h"

class Registers
{
public:
    static const int POS_N = 7;
    static const int POS_V = 6;
    static const int POS_B = 4;
    static const int POS_D = 3;
    static const int POS_I = 2;
    static const int POS_Z = 1;
    static const int POS_C = 0;

    u8 A;
    u8 X;
    u8 Y;
    u16 PC;
    u8 SP;
    u8 P;
    Registers();

    void setP(int position, bool value);
    bool getP(int position);

    void setNZ(u8 value);
    bool carry(u8 a, u8 b);
    bool carry(u8 a, u8 b, u8 c);
};

#endif // REGISTERS_H
