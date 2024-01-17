#include "cpu.h"

CPU::CPU()
{
    this->registers_ = new Registers();

}

CPU::~CPU() {
    delete this->registers_;
}

void CPU::bindBus(Bus *bus) {
    bus_ = bus;
}

Instr* CPU::fetch() {
    u8 instr = bus_->read(registers_->PC);
    registers_->PC++;
    u8 high = instr & (0xf0);
    u8 low = instr & (0x0f);

    return InstrGenerator::generateInstr(this->registers_, this->bus_, high, low);
}

void CPU::execute() {

    if(cycle_ > 0) {
        cycle_--;
        return;
    }
    Instr* instr = fetch();
    cycle_ = instr->execute();

}
