#include "machine.h"

Machine::Machine()
{
    this->cpu = new CPU;
    this->bus = new Bus;
    this->memory = new Memory;

    this->cpu->bindBus(bus);
    this->bus->bindMemory(memory);
}

Machine::~Machine() {
    delete cpu;
    delete bus;
    delete memory;
}
