#include "machine.h"
#include "rom.h"
#include <iostream>
#include <assert.h>

Machine::Machine()
{
    this->cpu = new CPU;
    this->bus = new Bus;
    this->ram = new MemoryBlock(0x800);
    this->ppu = new PPU();
    this->ppuBus = new PPUBus();

    this->cpu->bindBus(bus);
    this->bus->bindRAM(ram);
    this->ppu->bindPPUBus(ppuBus);

}

Machine::~Machine() {
    delete cpu;
    delete bus;
    delete ram;
}

void Machine::reset() {
    this->cpu->reset();
}

void Machine::load(const std::string& path) {
    ROM rom;
    bool loadSuccess = rom.load(path);
    if(!loadSuccess) {
        std::cout << "fail to load rom file" << std::endl;
        assert(false);
        return;
    }

    // load PRG
    u16 prgSize = rom.getPRGSize();
    this->prg = new MemoryBlock(prgSize);
    for(int i = 0; i < prgSize; i++) {
        this->prg->m_[i] = rom.PRG_[i];
    }

    bus->bindPRG(prg);

    // load CHR
    u16 chrSize = rom.getCHRSize();


    reset();
}

void Machine::update() {
    this->cpu->execute();
}
