#include "cpu.h"
#include <deque>
#include <iostream>
#include <list>

CPU::CPU()
{
    this->registers_ = new Registers();
}

CPU::~CPU() {
    delete this->registers_;
}

void CPU::reset() {
    registers_->A = 0;
    registers_->X = 0;
    registers_->Y = 0;

    registers_->SP = 0xfd;
    registers_->P = 0;
    registers_->setP(Registers::POS_I, 1);
    registers_->setP(Registers::POS_ONE, 1);

    u8 low = bus_->read(0xFFFC);
    u8 high = bus_->read(0xFFFD);
    registers_->PC = u16(high << 8) + low;

    cycle_ = 0;
    clockCount_ = 0;
}

void CPU::bindBus(Bus *bus) {
    bus_ = bus;
}

Instr* CPU::fetch() {
    u8 instr = bus_->read(registers_->PC);
    registers_->PC++;
    u8 high = (instr & (0xf0)) >> 4;
    u8 low = instr & (0x0f);

    return InstrGenerator::generateInstr(high, low);
}

void CPU::execute() {
    static std::list<u16> lastPC(0);
    static std::list<std::string> lastInstr(0);
    clockCount_++;
    if(cycle_ > 0) {
        cycle_--;
        return;
    }
    // if(this->registers_->PC < 0x8000) {
    //     int a = 0;
    // }
    // if(this->registers_->PC == 0x8e96) {
    //     int a = 0;
    // }

    lastPC.push_back(registers_->PC);
    if(lastPC.size() > 100) {
        lastPC.pop_front();
    }
    u8 tmp = bus_->read(registers_->PC);
    u8 high = (tmp & (0xf0)) >> 4;
    u8 low = tmp & (0x0f);
    lastInstr.push_back(InstrGenerator::getInstrName(high, low));
    if(lastInstr.size() > 100) {
        lastInstr.pop_front();
    }

    // std::cout << this->registers_->PC << std::endl;
    Instr* instr = fetch();
    cycle_ += instr->execute(registers_, bus_);

    cycle_--;
}

void CPU::nmi() {
    cycle_ = instr6502::NMI(registers_, bus_);
    cycle_--;
}

void CPU::oamCycle() {
    cycle_ += 513 + clockCount_ & 1;
//    cycle_ += 512 + clockCount_ & 1;
}
