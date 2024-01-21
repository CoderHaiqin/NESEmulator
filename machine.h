#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"

class Machine
{
private:
    CPU* cpu;
    Bus* bus;
    MemoryBlock* ram;

    MemoryBlock* prg;

public:
    Machine();
    ~Machine();

    void reset();
    void load(const std::string& path);
    void update();
};

#endif // MACHINE_H
