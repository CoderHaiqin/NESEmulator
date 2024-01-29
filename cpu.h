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

    int cycle_;
    u32 clockCount_;

public:
    CPU();
    ~CPU();

    void reset();

    Instr* fetch();
    void execute();

    void bindBus(Bus* bus);
    void nmi();

    void oamCycle();
};

#endif // CPU_H
