#include "machine.h"

Machine::Machine()
{
    this->cpu = new CPU;
    this->bus = new Bus;
    this->ram = new MemoryBlock(0x800);

    this->cpu->bindBus(bus);
    this->bus->bindRAM(ram);
}

Machine::~Machine() {
    delete cpu;
    delete bus;
    delete ram;
}
