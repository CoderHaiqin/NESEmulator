
#include "instr.h"
#include "constant.h"
#include <iostream>

Instr::Instr(Registers* registers, Bus* bus) : registers_(registers), bus_(bus)
{
    func_ = [](Registers* registers, Bus* bus, int access) {
        std::cout << "invalid instrction" << std::endl;
        return 0;
    };
}

Instr::Instr(Registers* registers, Bus* bus, std::function<int(Registers* registers, Bus* bus, int access)> func) : registers_(registers), bus_(bus), func_(func) {

}

int Instr::execute() {
    return func_(this->registers_, this->bus_, this->access_);
}

void InstrGenerator::init() {
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
            table[i][j] = [](Registers* registers, Bus* bus) -> int {

            };
        }
    }
}


namespace instr6502 {
int BRK(Registers* registers, Bus* bus, int access) {

}

int LDA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    registers->A = bus->read(addr);

    return cycle;
}

int LDX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    registers->X = bus->read(addr);

    return cycle;
}

int LDY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    registers->Y = bus->read(addr);

    return cycle;
}

int STA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    bus->write(addr, registers->A);

    return cycle;
}

int STX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    bus->write(addr, registers->X);

    return cycle;
}

int STY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    bus->write(addr, registers->Y);

    return cycle;
}

int STZ(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);
    bus->write(addr, 0);

    return cycle;
}

int PHA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    registers->SP -= 8;
    addr = registers->SP | (0x0100);
    bus->write(addr, registers->A);

    return cycle;
}

int PHX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    registers->SP -= 8;
    addr = registers->SP | (0x0100);
    bus->write(addr, registers->X);

    return cycle;
}

int PHY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    registers->SP -= 8;
    addr = registers->SP | (0x0100);
    bus->write(addr, registers->Y);

    return cycle;
}

int PHP(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    registers->SP -= 8;
    addr = registers->SP | (0x0100);
    bus->write(addr, registers->P);

    return cycle;
}

int PLA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);
    registers->A = bus->read(addr);
    registers->SP += 8;

    return cycle;
}

int PLX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);
    registers->X = bus->read(addr);
    registers->SP += 8;

    return cycle;
}

int PLY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);
    registers->Y = bus->read(addr);
    registers->SP += 8;

    return cycle;
}

int PLP(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);
    registers->P = bus->read(addr);
    registers->SP += 8;

    return cycle;
}

int TSX(Registers* registers, Bus* bus, int access) {
    registers->X = registers->SP;

    return 0;
}

int TXS(Registers* registers, Bus* bus, int access) {
    registers->SP = registers->X;

    return 0;
}

int INA(Registers* registers, Bus* bus, int access) {
    registers->A++;

    return 0;
}

int INX(Registers* registers, Bus* bus, int access) {
    registers->X++;

    return 0;
}

int INY(Registers* registers, Bus* bus, int access) {
    registers->Y++;

    return 0;
}

int DEA(Registers* registers, Bus* bus, int access) {
    registers->A--;

    return 0;
}

int DEX(Registers* registers, Bus* bus, int access) {
    registers->X--;

    return 0;
}

int DEY(Registers* registers, Bus* bus, int access) {
    registers->Y--;

    return 0;
}


int INC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    value++;
    bus->write(addr, value);

    return cycle;
}

int DEC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    value--;
    bus->write(addr, value);

    return cycle;
}

int ASL(Registers* registers, Bus* bus, int access) {
    u8 value = registers->A;
    u8 high = value & (0x80);

    value = value << 1;
    registers->A = value;

    return 0;
}

int LSR(Registers* registers, Bus* bus, int access) {
    u8 value = registers->A;
    u8 low = value & (0x01);

    value = value >> 1;
    registers->A = value;

    return 0;
}

int ROL(Registers* registers, Bus* bus, int access) {
    if(access == 0) {
        u8 value = registers->A;
        u8 C = registers->P & (0x01);
        value = value << 1;
        value = value | C;
        registers->A = value;

        return 0;
    }

    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    u8 C = registers->P & (0x01);
    value = value << 1;
    value = value | C;
    bus->write(addr, value);

    return cycle;
}

int ROR(Registers* registers, Bus* bus, int access) {
    if(access == 0) {
        u8 value = registers->A;
        u8 C = (registers->P & (0x01)) << 7;
        value = value >> 1;
        value = value | C;
        registers->A = value;

        return 0;
    }

    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    u8 C = (registers->P & (0x01)) << 7;
    value = value >> 1;
    value = value | C;
    bus->write(addr, value);

    return cycle;
}

int AND(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A & value;

    return cycle;
}

int ORA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A | value;

    return cycle;
}

int EOR(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A ^ value;

    return cycle;
}

int BIT(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    bool testResult = !(registers->A & value);
    registers->setP(Registers::POS_Z, testResult);
    registers->setP(Registers::POS_N, (value >> 7) & 0x1);
    registers->setP(Registers::POS_V, (value >> 6) & 0x1);

    return value;
}

int CMP(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->setP(Registers::POS_N, (value - registers->A) >> 7);

    if(registers->A < value) {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 0);
    } else if(registers->A == value) {
        registers->setP(Registers::POS_Z, 1);
        registers->setP(Registers::POS_C, 1);
    } else {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 1);
    }

    return cycle;

}

int CPX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->setP(Registers::POS_N, (value - registers->X) >> 7);

    if(registers->X < value) {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 0);
    } else if(registers->X == value) {
        registers->setP(Registers::POS_Z, 1);
        registers->setP(Registers::POS_C, 1);
    } else {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 1);
    }

    return cycle;
}

int CPY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->setP(Registers::POS_N, (value - registers->Y) >> 7);

    if(registers->Y < value) {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 0);
    } else if(registers->Y == value) {
        registers->setP(Registers::POS_Z, 1);
        registers->setP(Registers::POS_C, 1);
    } else {
        registers->setP(Registers::POS_Z, 0);
        registers->setP(Registers::POS_C, 1);
    }

    return cycle;
}

int ADC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A + value + registers->getP(Registers::POS_C);

    return cycle;
}

int SBC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
}

}

namespace access6502{

u16 impliedAccess(Registers* registers, Bus* bus, int& cycle) {
    cycle = 0;

    return 0;
}

u16 immediateAccess(Registers* registers, Bus* bus, int& cycle) {
    u16 addr = registers->PC;

    cycle = 0;
    return addr;
}

u16 zeropageAccess(Registers* registers, Bus* bus, char regis, int& cycle) {
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

u16 zeropageAccessX(Registers* registers, Bus* bus, int& cycle) {
    return zeropageAccess(registers, bus, 'X', cycle);
}

u16 zeropageAccessY(Registers* registers, Bus* bus, int& cycle) {
    return zeropageAccess(registers, bus, 'Y', cycle);
}

u16 absoluteAccess(Registers* registers, Bus* bus, char regis, int& cycle) {
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

u16 absoluteAccessX(Registers* registers, Bus* bus, int& cycle) {
    return absoluteAccess(registers, bus, 'X', cycle);
}

u16 absoluteAccessY(Registers* registers, Bus* bus, int& cycle) {
    return absoluteAccess(registers, bus, 'Y', cycle);
}

u16 indirectAccess(Registers* registers, Bus* bus, int& cycle) {
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

u16 indirectAccessX(Registers* registers, Bus* bus, int& cycle) {
    u8 offset = bus->read(registers->PC) + registers->X;
    registers->PC++;

    u8 low = bus->read(0x00ff & offset);
    u8 high = bus->read(0x00ff & (offset + 1));

    u16 targetAddr = (((u16)(high)) << 8) & low;

    cycle = 0;
    return bus->read(targetAddr);
}

u16 indirectAccessY(Registers* registers, Bus* bus, int& cycle) {
    u8 offset = bus->read(registers->PC);
    registers->PC++;

    u8 low = bus->read(0x00ff & offset);
    u8 high = bus->read(0x00ff & (offset + 1));

    u16 targetAddr = (((u16)(high)) << 8) & low;
    targetAddr += registers->Y;
    cycle = ((targetAddr & 0xff00) >> 8) != high;

    return bus->read(targetAddr);
}

u16 relativeAccess(Registers* registers, Bus* bus, int& cycle) {

    u16 offset = bus->read(registers->PC);
    registers->PC++;
    if(offset & (0x0080)) {
        offset |= 0xff00;
    }
    registers->PC += offset;

    cycle = 0;
    return 0;
}

std::function<u16(Registers* registers, Bus* bus, int& cycle)> getAccessFunc(int access) {
    static std::vector<std::function<u16(Registers* registers, Bus* bus, int& cycle)>> table = {
        impliedAccess, immediateAccess, zeropageAccess, absoluteAccess, indirectAccess,
        indirectAccessX, indirectAccessY, relativeAccess
    };

    if(access < 0 || access >= table.size()) {
        std::cout << "invalid access func" << std::endl;
        return [](Registers* registers, Bus* bus, int& cycle) -> u16 {
        };
    }
    return table[access];
}

}

