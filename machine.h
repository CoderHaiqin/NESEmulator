#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"

class Machine
{
private:
    CPU* cpu;
    Bus* bus;
    Memory* memory;

public:
    Machine();
    ~Machine();
};

#endif // MACHINE_H
