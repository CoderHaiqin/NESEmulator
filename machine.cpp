#include "machine.h"
#include "rom.h"
#include <iostream>
#include <assert.h>
#include <QDebug>

Machine::Machine() : cpu(), bus(), ppu(), ppuBus()
{
    this->ram = new MemoryBlock(0x800);
    this->extended_ram = new MemoryBlock(0x2000);

    this->cpu.bindBus(&bus);

    this->bus.bindRAM(ram);
    this->bus.bindExtendedRAM(extended_ram);
    this->bus.bindPPU(&ppu);
    this->bus.bindIORegister(&ioRegister);
    this->bus.bindCPU(&cpu);

    this->ppu.bindPPUBus(&ppuBus);
    this->ppu.bindCPU(&cpu);
    this->ioRegister.bindKeyState(keyState);

    for(int i = 0; i < 0x10; i++) {
        keyState[i] = 0;
    }
}

Machine::~Machine() {
}

void Machine::reset() {
    this->cpu.reset();
}

void Machine::load(const std::string& path) {
    bool loadSuccess = rom.load(path);
    if(!loadSuccess) {
        std::cout << "fail to load rom file" << std::endl;
        assert(false);
        return;
    }

    mapper = Mapper::createFromROM(&rom);
    bus.bindMapper(mapper);
    ppuBus.bindMapper(mapper);

    reset();
}

void Machine::update() {
    for (int i = 0; i < 29781; ++i) //per frame
    {
//        ppu.execute();
        ppu.execute();
        ppu.execute();
        cpu.execute();
    }

}

void Machine::get(QRgb* pixels) {
//    ppu.get();
    for(int i = 0; i < 240; i++) {
        for(int j = 0; j < 256; j++) {
            pixels[256 * i + j] = qRgb(ppu.screen[i][j][0], ppu.screen[i][j][1], ppu.screen[i][j][2]);
            //pixels[256 * i + j] = qRgb(ppu.chrScreen[i][j][0], ppu.chrScreen[i][j][1], ppu.chrScreen[i][j][2]);

        }
    }
}

void Machine::input(int key, bool flag) {
    keyState[key] = flag;
}
