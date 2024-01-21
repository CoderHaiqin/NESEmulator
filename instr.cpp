
#include "instr.h"
#include "constant.h"
#include <iostream>
#include <sstream>
#include <fstream>

Instr::Instr(int access, int cycleNum, std::function<int(Registers* registers, Bus* bus, int access)> func)
    : access_(access), cycleNum_(cycleNum), func_(func) {
}

int Instr::execute(Registers* registers, Bus* bus) {
    return cycleNum_ + func_(registers, bus, this->access_);
}

InstrGenerator::InstrGenerator() {

}

InstrGenerator::InstrGenerator(const std::string& path) {
    instrNameTable["BRK"] = instr6502::BRK;
    instrNameTable["LDA"] = instr6502::LDA;
    instrNameTable["LDX"] = instr6502::LDX;
    instrNameTable["LDY"] = instr6502::LDY;
    instrNameTable["STA"] = instr6502::STA;
    instrNameTable["STX"] = instr6502::STX;
    instrNameTable["STY"] = instr6502::STY;
    instrNameTable["STZ"] = instr6502::STZ;
    instrNameTable["PHA"] = instr6502::PHA;
    instrNameTable["PHX"] = instr6502::PHX;
    instrNameTable["PHY"] = instr6502::PHY;
    instrNameTable["PHP"] = instr6502::PHP;
    instrNameTable["PLA"] = instr6502::PLA;
    instrNameTable["PLX"] = instr6502::PLX;
    instrNameTable["PLY"] = instr6502::PLY;
    instrNameTable["PLP"] = instr6502::PLP;
    instrNameTable["TSX"] = instr6502::TSX;
    instrNameTable["TXS"] = instr6502::TXS;
    instrNameTable["INA"] = instr6502::INA;
    instrNameTable["INX"] = instr6502::INX;
    instrNameTable["INY"] = instr6502::INY;
    instrNameTable["DEA"] = instr6502::DEA;
    instrNameTable["DEX"] = instr6502::DEX;
    instrNameTable["DEY"] = instr6502::DEY;
    instrNameTable["INC"] = instr6502::INC;
    instrNameTable["DEC"] = instr6502::DEC;
    instrNameTable["ASL"] = instr6502::ASL;
    instrNameTable["LSR"] = instr6502::LSR;
    instrNameTable["ROL"] = instr6502::ROL;
    instrNameTable["ROR"] = instr6502::ROR;
    instrNameTable["AND"] = instr6502::AND;
    instrNameTable["ORA"] = instr6502::ORA;
    instrNameTable["EOR"] = instr6502::EOR;
    instrNameTable["BIT"] = instr6502::BIT;
    instrNameTable["CMP"] = instr6502::CMP;
    instrNameTable["CPX"] = instr6502::CPX;
    instrNameTable["CPY"] = instr6502::CPY;
    instrNameTable["ADC"] = instr6502::ADC;
    instrNameTable["SBC"] = instr6502::SBC;
    instrNameTable["JMP"] = instr6502::JMP;
    instrNameTable["JSR"] = instr6502::JSR;
    instrNameTable["RTX"] = instr6502::RTX;
    instrNameTable["RTI"] = instr6502::RTI;
    instrNameTable["BEQ"] = instr6502::BEQ;
    instrNameTable["BNE"] = instr6502::BNE;
    instrNameTable["BCS"] = instr6502::BCS;
    instrNameTable["BCC"] = instr6502::BCC;
    instrNameTable["BMI"] = instr6502::BMI;
    instrNameTable["BPL"] = instr6502::BPL;
    instrNameTable["BVS"] = instr6502::BVS;
    instrNameTable["BVC"] = instr6502::BVC;
    instrNameTable["CLC"] = instr6502::CLC;
    instrNameTable["CLD"] = instr6502::CLD;
    instrNameTable["CLI"] = instr6502::CLI;
    instrNameTable["CLV"] = instr6502::CLV;
    instrNameTable["SEC"] = instr6502::SEC;
    instrNameTable["SED"] = instr6502::SED;
    instrNameTable["SEI"] = instr6502::SEI;
    instrNameTable["TAX"] = instr6502::TAX;
    instrNameTable["TAY"] = instr6502::TAY;
    instrNameTable["TXA"] = instr6502::TXA;
    instrNameTable["TYA"] = instr6502::TYA;
    instrNameTable["NOP"] = instr6502::NOP;

    std::ifstream f(path);
    if(!f.is_open()) {
        std::cout << "fail to open file" << std::endl;
        return;
    }

    std::string buf;
    int count = 0;
    std::string t[3 * Constant::instrTableSize][Constant::instrTableSize];
    while(std::getline(f, buf)) {
        if(buf.empty()) {
            continue;
        }
        std::stringstream ss(buf);
        for(int i = 0; i < Constant::instrTableSize; i++) {
            std::string tmp;
            ss >> tmp;
            t[count][i] = tmp;
        }
        count++;
    }

    accessTypeTable.resize(Constant::instrTableSize);
    cycleNumTable.resize(Constant::instrTableSize);
    funcTable.resize(Constant::instrTableSize);
    instrTable.resize(Constant::instrTableSize);
    for(int i = 0; i < Constant::instrTableSize; i++) {
        cycleNumTable[i].resize(Constant::instrTableSize);
        accessTypeTable[i].resize(Constant::instrTableSize);
        funcTable[i].resize(Constant::instrTableSize);
        instrTable[i].resize(Constant::instrTableSize);
    }


    for(int i = 0; i < Constant::instrTableSize; i++) {
        for(int j = 0; j < Constant::instrTableSize; j++) {
            accessTypeTable[i][j] = atoi(t[i][j].c_str());
            cycleNumTable[i][j] = atoi(t[i + Constant::instrTableSize][j].c_str());

            funcTable[i][j] = instrNameTable[t[i + 2 * Constant::instrTableSize][j]];
        }
    }

    for(int i = 0; i < Constant::instrTableSize; i++) {
        for(int j = 0; j < Constant::instrTableSize; j++) {
            instrTable[i][j] = new Instr(accessTypeTable[i][j], cycleNumTable[i][j], funcTable[i][j]);
        }
    }
}

Instr* InstrGenerator::generateInstr(u8 high, u8 low) {
    static InstrGenerator g("./instr.txt");

    return g.instrTable[high][low];
}

namespace instr6502 {
int BRK(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    accessAddr(registers, bus, cycle);

    registers->SP--;
    bus->write(registers->SP, (registers->PC & 0xff00) >> 8);
    registers->SP--;
    bus->write(registers->SP, registers->PC & 0x00ff);
    registers->SP--;
    bus->write(registers->SP, registers->P);

    u16 low = bus->read(0xfffe);
    u16 high = bus->read(0xffff);
    registers->PC = (high << 8) | low;

    registers->setP(Registers::POS_B, 1);

    return cycle;
}

int LDA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);

    registers->A = bus->read(value);
    registers->setNZ(value);

    return cycle;
}

int LDX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);

    registers->X = value;
    registers->setNZ(value);

    return cycle;
}

int LDY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);

    registers->Y = value;
    registers->setNZ(value);

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

    u8 value = bus->read(addr);
    registers->setNZ(value);

    registers->A = value;
    registers->SP += 8;

    return cycle;
}

int PLX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);

    u8 value = bus->read(addr);
    registers->setNZ(value);

    registers->X = value;
    registers->SP += 8;

    return cycle;
}

int PLY(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    addr = registers->SP | (0x0100);

    u8 value = bus->read(addr);
    registers->setNZ(value);

    registers->Y = value;
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

    registers->setNZ(registers->X);

    return 0;
}

int TXS(Registers* registers, Bus* bus, int access) {
    registers->SP = registers->X;

    return 0;
}

int INA(Registers* registers, Bus* bus, int access) {
    registers->A++;

    registers->setNZ(registers->A);

    return 0;
}

int INX(Registers* registers, Bus* bus, int access) {
    registers->X++;

    registers->setNZ(registers->X);

    return 0;
}

int INY(Registers* registers, Bus* bus, int access) {
    registers->Y++;

    registers->setNZ(registers->Y);

    return 0;
}

int DEA(Registers* registers, Bus* bus, int access) {
    registers->A--;

    registers->setNZ(registers->A);

    return 0;
}

int DEX(Registers* registers, Bus* bus, int access) {
    registers->X--;

    registers->setNZ(registers->X);

    return 0;
}

int DEY(Registers* registers, Bus* bus, int access) {
    registers->Y--;

    registers->setNZ(registers->Y);

    return 0;
}


int INC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    value++;

    registers->setNZ(value);

    bus->write(addr, value);

    return cycle;
}

int DEC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    value--;

    registers->setNZ(value);

    bus->write(addr, value);

    return cycle;
}

int ASL(Registers* registers, Bus* bus, int access) {
    u8 value = registers->A;
    u8 high = value & (0x80);

    value = value << 1;

    registers->setNZ(value);

    registers->A = value;

    return 0;
}

int LSR(Registers* registers, Bus* bus, int access) {
    u8 value = registers->A;
    u8 low = value & (0x01);

    value = value >> 1;

    registers->setNZ(value);

    registers->A = value;

    return 0;
}

int ROL(Registers* registers, Bus* bus, int access) {
    if(access == 0) {
        u8 value = registers->A;
        u8 C = registers->P & (0x01);
        value = value << 1;
        value = value | C;

        registers->setNZ(value);

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

    registers->setNZ(value);

    bus->write(addr, value);

    return cycle;
}

int ROR(Registers* registers, Bus* bus, int access) {
    if(access == 0) {
        u8 value = registers->A;
        u8 C = (registers->P & (0x01)) << 7;
        value = value >> 1;
        value = value | C;

        registers->setNZ(value);

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

    registers->setNZ(value);

    bus->write(addr, value);

    return cycle;
}

int AND(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A & value;

    registers->setNZ(registers->A);

    return cycle;
}

int ORA(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A | value;

    registers->setNZ(registers->A);

    return cycle;
}

int EOR(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    registers->A = registers->A ^ value;

    registers->setNZ(registers->A);

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

    u8 prevA = registers->A;
    registers->setP(Registers::POS_C, registers->carry(prevA, value, registers->getP(Registers::POS_C)));
    registers->A = registers->A + value + registers->getP(Registers::POS_C);

    registers->setNZ(registers->A);
    registers->setP(Registers::POS_V, (prevA & 0x8000) != (registers->A & 0x8000));

    return cycle;
}

int SBC(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);
    u8 prevA = registers->A;
    registers->setP(Registers::POS_C, registers->carry(prevA, -value, -(~(u8)registers->getP(Registers::POS_C))));
    registers->A = registers->A - value - (~(u8)registers->getP(Registers::POS_C));

    registers->setNZ(registers->A);
    registers->setP(Registers::POS_V, (prevA & 0x8000) != (registers->A & 0x8000));

    return cycle;
}

int JMP(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);

    registers->PC = value;

    return cycle;
}

int JSR(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u8 value = bus->read(addr);

    registers->SP--;
    bus->write(registers->SP, (registers->PC & 0xff00) >> 8);
    registers->SP--;
    bus->write(registers->SP, registers->PC & 0x00ff);

    registers->PC = value;

    return cycle;
}

int RTX(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u16 prevPC = 0;
    u16 low = bus->read(registers->SP);
    registers->SP++;
    u16 high = bus->read(registers->SP);
    registers->SP++;
    prevPC = (high << 8) | low;

    registers->PC = prevPC;
}

int RTI(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    u16 addr = accessAddr(registers, bus, cycle);

    u16 prevPC = 0;
    u8 prevP = bus->read(registers->SP);
    registers->SP++;
    u16 low = bus->read(registers->SP);
    registers->SP++;
    u16 high = bus->read(registers->SP);
    registers->SP++;

    registers->P = prevP;
    prevPC = (high << 8) | low;

    registers->PC = prevPC;
}

int Branch(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    accessAddr(registers, bus, cycle);

    return cycle;

}

int BEQ(Registers* registers, Bus* bus, int access) {
    if(registers->getP(Registers::POS_Z)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BNE(Registers* registers, Bus* bus, int access) {
    if(!registers->getP(Registers::POS_Z)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BCS(Registers* registers, Bus* bus, int access) {
    if(registers->getP(Registers::POS_C)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BCC(Registers* registers, Bus* bus, int access) {
    if(!registers->getP(Registers::POS_C)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BMI(Registers* registers, Bus* bus, int access) {
    if(registers->getP(Registers::POS_N)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BPL(Registers* registers, Bus* bus, int access) {
    if(!registers->getP(Registers::POS_N)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BVS(Registers* registers, Bus* bus, int access) {
    if(registers->getP(Registers::POS_V)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int BVC(Registers* registers, Bus* bus, int access) {
    if(!registers->getP(Registers::POS_V)) {
        return Branch(registers, bus, access);
    }
    registers->PC++;
    return 0;
}

int CLC(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_C, 0);
    return 0;
}

int CLD(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_D, 0);
    return 0;
}

int CLI(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_I, 0);
    return 0;
}

int CLV(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_V, 0);
    return 0;
}

int SEC(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_C, 1);
    return 0;
}

int SED(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_D, 1);
    return 0;
}

int SEI(Registers* registers, Bus* bus, int access) {
    registers->setP(Registers::POS_I, 1);
    return 0;
}

int TAX(Registers* registers, Bus* bus, int access) {
    registers->X = registers->A;

    registers->setNZ(registers->A);
    return 0;
}

int TAY(Registers* registers, Bus* bus, int access) {
    registers->Y = registers->A;
    registers->setNZ(registers->A);
    return 0;
}

int TXA(Registers* registers, Bus* bus, int access) {
    registers->A = registers->X;
    registers->setNZ(registers->A);
    return 0;
}

int TYA(Registers* registers, Bus* bus, int access) {
    registers->A = registers->Y;
    registers->setNZ(registers->A);
    return 0;
}

int NOP(Registers* registers, Bus* bus, int access) {
    auto accessAddr = access6502::getAccessFunc(access);
    int cycle = 0;
    accessAddr(registers, bus, cycle);

    return cycle;
}

}

namespace access6502{

u16 impliedAccess(Registers* registers, Bus* bus, int& cycle) {
    cycle = 0;

    return 0;
}

u16 immediateAccess(Registers* registers, Bus* bus, int& cycle) {
    u16 addr = registers->PC;
    registers->PC++;

    cycle = 0;
    return addr;
}

u16 zeropageAccess(Registers* registers, Bus* bus, char regis, int& cycle) {
    u16 addr = registers->PC;
    u8 offset = bus->read(addr);
    registers->PC++;
    switch (regis) {
    case 'X' : offset += registers->X;break;
    case 'Y' : offset += registers->Y;break;
    case '0' : break;
    default: std::cout << "zero page access error: invalid register";break;
    }

    cycle = 0;

    return 0x00ff & offset;
}

u16 zeropageAccessNone(Registers* registers, Bus* bus, int& cycle) {
    return zeropageAccess(registers, bus, '0', cycle);
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

    return targetAddr;
}

u16 absoluteAccessNone(Registers* registers, Bus* bus, int& cycle) {
    return absoluteAccess(registers, bus, '0', cycle);
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
    return targetAddr;
}

u16 indirectAccessY(Registers* registers, Bus* bus, int& cycle) {
    u8 offset = bus->read(registers->PC);
    registers->PC++;

    u8 low = bus->read(0x00ff & offset);
    u8 high = bus->read(0x00ff & (offset + 1));

    u16 targetAddr = (((u16)(high)) << 8) & low;
    targetAddr += registers->Y;
    cycle = ((targetAddr & 0xff00) >> 8) != high;

    return targetAddr;
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

u16 indirectAbsoluteAccess(Registers* registers, Bus* bus, int& cycle) {
    u16 addr1 = registers->PC;
    registers->PC++;
    u8 low = bus->read(addr1);

    u16 addr2 = registers->PC;
    registers->PC++;
    u8 high = bus->read(addr2);

    u16 targetAddr = (((u16)(high)) << 8) & low;
    return bus->read(targetAddr);
}

std::function<u16(Registers* registers, Bus* bus, int& cycle)> getAccessFunc(int access) {
    /*
    *   0 imp
    *   1 imm
    *   2 z
    *   3 zx
    *   4 zy
    *   5 a
    *   6 ax
    *   7 ay
    *   8 i
    *   9 ix
    *   10 iy
    *   11 r
    *   12 iab
    */
    static std::vector<std::function<u16(Registers* registers, Bus* bus, int& cycle)>> table = {
        impliedAccess, immediateAccess, zeropageAccessNone, zeropageAccessX, zeropageAccessY,
        absoluteAccessNone, absoluteAccessX, absoluteAccessY,
        indirectAccess, indirectAccessX, indirectAccessY, relativeAccess, indirectAbsoluteAccess
    };

    if(access < 0 || access >= table.size()) {
        std::cout << "invalid access func" << std::endl;
        return [](Registers* registers, Bus* bus, int& cycle) -> u16 {
        };
    }
    return table[access];
}

}

