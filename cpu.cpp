#include "cpu.h"
#include <iostream>

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

    if(cycle_ > 0) {
        cycle_--;
        return;
    }
    // std::cout << this->registers_->PC << std::endl;
    Instr* instr = fetch();
    cycle_ = instr->execute(registers_, bus_);

    cycle_--;
}
