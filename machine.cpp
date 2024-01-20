#include "machine.h"

Machine::Machine()
{
    this->cpu = new CPU;
    this->bus = new Bus;
    this->ram = new RAM;

    this->cpu->bindBus(bus);
    this->bus->bindRAM(ram);
}

Machine::~Machine() {
    delete cpu;
    delete bus;
    delete ram;
}
