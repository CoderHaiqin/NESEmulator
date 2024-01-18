#include "registers.h"

Registers::Registers() : A(0), X(0), Y(0), PC(0), SP(0), P(0)
{

}

bool Registers::getP(int position) {
    if(position < 0 || position >= 8) {
        return 0;
    }
    return (this->P >> position) & 0x1;
}

void Registers::setP(int position, bool value) {
    if(position < 0 || position >= 8) {
        return;
    }

    if(value) {
        this->P = this->P | (0x1 << position);
    } else {
        this->P = this->P & ~(0x1 << position);
    }
}

void Registers::setNZ(u8 value) {
    setP(Registers::POS_N, value & (0x8000));
    setP(Registers::POS_Z, value == 0);
}

bool Registers::carry(u8 a, u8 b) {
    u8 bit1 = (a & 0x8000) != 0;
    u8 bit2 = (b & 0x8000) != 0;
    u8 bit3 = ((a + b) & 0x8000) != 0;

    if((bit1 & bit2) || ((bit1 ^ bit2) & bit3)) {
        return true;
    } else {
        return false;
    }
}

bool Registers::carry(u8 a, u8 b, u8 c) {
    return carry(b, c) || carry(a, b + c);
}
