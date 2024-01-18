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
    int access_;
    std::function<int(Registers* registers, Bus* bus, int access)> func_;

public:
    Instr(Registers* registers, Bus* bus);
    Instr(Registers* registers, Bus* bus, std::function<int(Registers* registers, Bus* bus, int access)> func);

    int execute();

};

class InstrGenerator {
public:
    static std::vector<std::vector<int>> cycleNumTable;
    static std::vector<std::vector<int>> instrTypeTable;

    static std::vector<std::vector<std::function<int(Registers* registers, Bus* bus)>>> table;
    static void init();
    static Instr* generateInstr(Registers* registers, Bus* bus, u8 high, u8 low);
};

namespace instr6502 {
}

namespace access6502{
std::function<u16(Registers* registers, Bus* bus, int& cycle)> getAccessFunc(int access);

}


#endif // INSTR_H
