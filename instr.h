#ifndef INSTR_H
#define INSTR_H
#include "registers.h"
#include "bus.h"
#include <vector>
#include <string>
#include <functional>

class Instr
{
private:
    Registers* registers_;
    Bus* bus_;
    std::function<void(Registers*, Bus*)> func_;
public:
    Instr(Registers* registers, Bus* bus);
    Instr(Registers* registers, Bus* bus, std::function<void(Registers*, Bus*)> func);

    bool execute();
};



class InstrGenerator {
public:
    static std::vector<std::vector<int>> cycleNumTable;
    static std::vector<std::vector<int>> instrTypeTable;

    static std::vector<std::vector<Instr*>> table;
    static void init(const std::string& pathName, Registers* registers, Bus* bus);
    static Instr* generateInstr(u8 high, u8 low);

    static u8 accumulatorAccess(u16 PC);
    static u8 ImpliedAccess(u16 PC);
    static u8 absoluteAccess(u16 PC);
    static u8 zeroPageAccess(u16 PC);
    static u8 relativeAccess(u16 PC);
    static u8 indirectAccess(u16 PC);
    static u8 zeroPageIndexAccess(u16 PC);


};

#endif // INSTR_H
