#include "instr.h"
#include "constant.h"
Instr::Instr(Registers* registers, Bus* bus) : registers(registers), bus(bus)
{

}

bool LoadInstr::execute() {

}

void InstrGenerator::init(Registers* registers, Bus* bus) {
    instrTypeTable.resize(Constant::instrTableSize);
    cycleNumTable.resize(Constant::instrTableSize);
    table.resize(Constant::instrTableSize);
    for(int i = 0; i < Constant::instrTableSize; i++) {
        cycleNumTable[i].resize(Constant::instrTableSize);
        instrTypeTable[i].resize(Constant::instrTableSize);
        table[i].resize(Constant::instrTableSize);
    }


    for(int i = 0; i < Constant::instrTableSize; i++) {
        for(int j = 0; j < Constant::instrTableSize; i++) {
            
        }
    }

    table[0][0] = new BreakInstr(registers, bus);
}
