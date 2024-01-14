#ifndef CPU_H
#define CPU_H
#include "bus.h"
#include "registers.h"
#include "instr.h"

class CPU
{
private:
    Registers* registers_;
    Bus* bus_;

    bool needFetch_;

    Instr* instr;

public:
    CPU();

    void fetch();
    void execute();

    void bindBus(Bus* bus);
};

#endif // CPU_H
