#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"

class Machine
{
private:
    CPU* cpu;
    Bus* bus;
    RAM* ram;

public:
    Machine();
    ~Machine();


};

#endif // MACHINE_H
