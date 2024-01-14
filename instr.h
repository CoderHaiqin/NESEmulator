#ifndef INSTR_H
#define INSTR_H
#include "registers.h"
#include "bus.h"
#include <vector>

class Instr
{
protected:
    Registers* registers;
    Bus* bus;

    Instr(Registers* registers, Bus* bus);

    virtual bool execute() = 0;
};

class LoadInstr : public Instr {
public:
    LoadInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class StoreInstr : public Instr {
public:
    StoreInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class StackInstr : public Instr {
public:
    StackInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class IncrementInstr : public Instr {
public:
    IncrementInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class ShiftInstr : public Instr {
public:
    ShiftInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class LogicInstr : public Instr {
public:
    LogicInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class ArithmeticInstr : public Instr {
public:
    ArithmeticInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class BranchInstr : public Instr {
public:
    bool execute() override;
};

class ConditionInstr : public Instr {
public:
    ConditionInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class TransferInstr : public Instr {
public:
    TransferInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class BreakInstr : public Instr {
public:
    BreakInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class IllegalInstr : public Instr {
public:
    IllegalInstr(Registers* registers, Bus* bus);
    bool execute() override;
};

class InstrGenerator {
public:
    static std::vector<std::vector<int>> cycleNumTable;
    static std::vector<std::vector<int>> instrTypeTable;

    static std::vector<std::vector<Instr*>> table;
    static void init(Registers* registers, Bus* bus);
    static Instr* generateInstr(u8 high, u8 low);
};

#endif // INSTR_H
