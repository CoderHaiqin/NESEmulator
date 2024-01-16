#ifndef INSTR_H
#define INSTR_H
#include "registers.h"
#include "bus.h"
#include <vector>
#include <functional>

class Instr
{
private:
    Registers* registers_;
    Bus* bus_;
    std::function<void(Registers* registers, Bus* bus)> func_;

public:

    Instr(Registers* registers, Bus* bus);
    Instr(Registers* registers, Bus* bus, std::function<void(Registers* registers, Bus* bus)> func);

    void execute();

    static void immediateAccess();
    static void indirectAccess(Bus* bus);
    static void absoluteAccess(Bus* bus);
    static void zeropageAccess(Registers* registers, Bus* bus, char regis);
    static void impliedAccess();
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
