
#include "instr.h"
#include "constant.h"
#include <iostream>

namespace instr6502 {
void BRK(Registers* registers, Bus* bus) {

}

void LD(Registers* registers, Bus* bus, char target, u8 value) {

}

void ST(Registers* registers, Bus* bus, char* target, u8 value) {

}

void STZ(Registers* registers, Bus* bus, char* target) {

}
}

namespace access6052{

u8 impliedAccess(Registers* registers, Bus* bus, int& cycle) {
    cycle = 0;

    return 0;
}

u8 immediateAccess(Registers* registers, Bus* bus, int& cycle) {
    u16 addr = registers->PC;

    cycle = 0;
    return bus->read(addr);
}

u8 zeropageAccess(Registers* registers, Bus* bus, char regis, int& cycle) {
    u16 addr = registers->PC;
    u8 offset = bus->read(addr);
    switch (regis) {
    case 'X' : offset += registers->X;break;
    case 'Y' : offset += registers->Y;break;
    case '0' : break;
    default: std::cout << "zero page access error: invalid register";break;
    }

    cycle = 0;

    return bus->read(0x00ff & offset);
}

u8 absoluteAccess(Registers* registers, Bus* bus, char regis, int& cycle) {
    u16 addr1 = registers->PC;
    registers->PC++;
    u8 low = bus->read(addr1);

    u16 addr2 = registers->PC;
    registers->PC++;
    u8 high = bus->read(addr2);

    u16 targetAddr = (((u16)(high)) << 8) & low;

    switch (regis) {
    case 'X' : targetAddr += registers->X; break;
    case 'Y' : targetAddr += registers->Y; break;
    case '0' : break;
    default: std::cout << "zero page access error: invalid register";break;
    }
    cycle = ((targetAddr & 0xff00) >> 8) != high;

    return bus->read(targetAddr);
}

u8 indirectAccess(Registers* registers, Bus* bus, int& cycle) {
    u16 addr1 = registers->PC;
    registers->PC++;
    u8 low = bus->read(addr1);

    u16 addr2 = registers->PC;
    registers->PC++;
    u8 high = bus->read(addr2);

    u16 targetAddr = (((u16)(high)) << 8) & low;

    low = bus->read(targetAddr);
    high = bus->read(targetAddr + 1);
    targetAddr = (((u16)(high)) << 8) & low;

    cycle = 0;
    return bus->read(targetAddr);
}

u8 indirectAccessX(Registers* registers, Bus* bus, int& cycle) {
    u8 offset = bus->read(registers->PC) + registers->X;
    registers->PC++;

    u8 low = bus->read(0x00ff & offset);
    u8 high = bus->read(0x00ff & (offset + 1));

    u16 targetAddr = (((u16)(high)) << 8) & low;

    cycle = 0;
    return bus->read(targetAddr);
}

u8 indirectAccessY(Registers* registers, Bus* bus, int& cycle) {
    u8 offset = bus->read(registers->PC);
    registers->PC++;

    u8 low = bus->read(0x00ff & offset);
    u8 high = bus->read(0x00ff & (offset + 1));

    u16 targetAddr = (((u16)(high)) << 8) & low;
    targetAddr += registers->Y;
    cycle = ((targetAddr & 0xff00) >> 8) != high;

    return bus->read(targetAddr);
}

u8 relativeAccess(Registers* registers, Bus* bus, int& cycle) {

    u16 offset = bus->read(registers->PC);
    registers->PC++;
    if(offset & (0x0080)) {
        offset |= 0xff00;
    }
    registers->PC += offset;

    cycle = 0;
    return 0;
}
}

Instr::Instr(Registers* registers, Bus* bus) : registers_(registers), bus_(bus)
{
    func_ = [](Registers* registers, Bus* bus) {
        std::cout << "invalid instrction" << std::endl;
        return 0;
    };
}

Instr::Instr(Registers* registers, Bus* bus, std::function<void(Registers* registers, Bus* bus)> func) : registers_(registers), bus_(bus), func_(func) {

}

void Instr::execute() {
    func_(this->registers_, this->bus_);
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

    for(int i = 0; i < Constant::instrTableSize; i++) {
        for(int j = 0; j < Constant::instrTableSize; j++) {
            table[i][j] = new Instr(registers, bus, [](Registers* registers, Bus* bus) {

            });
        }
    }
}
