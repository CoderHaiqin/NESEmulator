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
