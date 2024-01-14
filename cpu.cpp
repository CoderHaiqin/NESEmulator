#include "cpu.h"

CPU::CPU()
{

}

void CPU::bindBus(Bus *bus) {
    bus_ = bus;
}

void CPU::fetch() {
    u8 instr = bus_->read(registers_->PC);
    u8 high = instr & (0xf0);
    u8 low = instr & (0x0f);
}

void CPU::execute() {
    // 6502 CPU is different from modern CPU,
    // it is possible to execute a single instr in multiple cycles
    // so we need to do fetch and execute seperately
    if(needFetch_) {
        fetch();
    }


}
