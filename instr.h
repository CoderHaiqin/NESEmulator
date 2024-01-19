#ifndef INSTR_H
#define INSTR_H
#include "registers.h"
#include "bus.h"
#include <vector>
#include <functional>
#include <map>
#include <string>

class Instr
{
private:
    int access_;
    int cycleNum_;
    std::function<int(Registers* registers, Bus* bus, int access)> func_;

public:
    Instr(int access, int cycleNum, std::function<int(Registers* registers, Bus* bus, int access)> func);

    int execute(Registers* registers, Bus* bus);

};

class InstrGenerator {
public:
    static std::map<std::string, std::function<int(Registers* registers, Bus* bus, int access)>> instrNameTable;

    static std::vector<std::vector<int>> cycleNumTable;
    static std::vector<std::vector<int>> accessTypeTable;

    static std::vector<std::vector<std::function<int(Registers* registers, Bus* bus, int access)>>> funcTable;

    static std::vector<std::vector<Instr*>> instrTable;

    static void init();
    static Instr* generateInstr(u8 high, u8 low);
};

namespace instr6502 {
int BRK(Registers* registers, Bus* bus, int access);

int LDA(Registers* registers, Bus* bus, int access);

int LDX(Registers* registers, Bus* bus, int access);

int LDY(Registers* registers, Bus* bus, int access);

int STA(Registers* registers, Bus* bus, int access);

int STX(Registers* registers, Bus* bus, int access);

int STY(Registers* registers, Bus* bus, int access);

int STZ(Registers* registers, Bus* bus, int access);

int PHA(Registers* registers, Bus* bus, int access);

int PHX(Registers* registers, Bus* bus, int access);

int PHY(Registers* registers, Bus* bus, int access);

int PHP(Registers* registers, Bus* bus, int access);

int PLA(Registers* registers, Bus* bus, int access);

int PLX(Registers* registers, Bus* bus, int access);

int PLY(Registers* registers, Bus* bus, int access);

int PLP(Registers* registers, Bus* bus, int access);

int TSX(Registers* registers, Bus* bus, int access);

int TXS(Registers* registers, Bus* bus, int access);

int INA(Registers* registers, Bus* bus, int access);

int INX(Registers* registers, Bus* bus, int access);

int INY(Registers* registers, Bus* bus, int access);

int DEA(Registers* registers, Bus* bus, int access);

int DEX(Registers* registers, Bus* bus, int access);

int DEY(Registers* registers, Bus* bus, int access);

int INC(Registers* registers, Bus* bus, int access);

int DEC(Registers* registers, Bus* bus, int access);

int ASL(Registers* registers, Bus* bus, int access);

int LSR(Registers* registers, Bus* bus, int access);

int ROL(Registers* registers, Bus* bus, int access);

int ROR(Registers* registers, Bus* bus, int access);

int AND(Registers* registers, Bus* bus, int access);

int ORA(Registers* registers, Bus* bus, int access);

int EOR(Registers* registers, Bus* bus, int access);

int BIT(Registers* registers, Bus* bus, int access);

int CMP(Registers* registers, Bus* bus, int access);

int CPX(Registers* registers, Bus* bus, int access);

int CPY(Registers* registers, Bus* bus, int access);

int ADC(Registers* registers, Bus* bus, int access);

int SBC(Registers* registers, Bus* bus, int access);

int JMP(Registers* registers, Bus* bus, int access);

int JSR(Registers* registers, Bus* bus, int access);

int RTX(Registers* registers, Bus* bus, int access);

int RTI(Registers* registers, Bus* bus, int access);

int Branch(Registers* registers, Bus* bus, int access);

int BEQ(Registers* registers, Bus* bus, int access);

int BNE(Registers* registers, Bus* bus, int access);

int BCS(Registers* registers, Bus* bus, int access);

int BCC(Registers* registers, Bus* bus, int access);

int BMI(Registers* registers, Bus* bus, int access);

int BPL(Registers* registers, Bus* bus, int access);

int BVS(Registers* registers, Bus* bus, int access);

int BVC(Registers* registers, Bus* bus, int access);

int CLC(Registers* registers, Bus* bus, int access);

int CLD(Registers* registers, Bus* bus, int access);

int CLI(Registers* registers, Bus* bus, int access);

int CLV(Registers* registers, Bus* bus, int access);

int SEC(Registers* registers, Bus* bus, int access);

int SED(Registers* registers, Bus* bus, int access);

int SEI(Registers* registers, Bus* bus, int access);

int TAX(Registers* registers, Bus* bus, int access);

int TAY(Registers* registers, Bus* bus, int access);

int TXA(Registers* registers, Bus* bus, int access);

int TYA(Registers* registers, Bus* bus, int access);

int NOP(Registers* registers, Bus* bus, int access);
}

namespace access6502{
std::function<u16(Registers* registers, Bus* bus, int& cycle)> getAccessFunc(int access);

}


#endif // INSTR_H
