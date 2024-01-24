#include "machine.h"
#include "rom.h"
#include <iostream>
#include <assert.h>
#include <qDebug>

Machine::Machine() : cpu(), bus(), ppu(), ppuBus()
{
    this->ram = new MemoryBlock(0x800);
    this->cpu.bindBus(&bus);
    this->bus.bindRAM(ram);
    this->ppu.bindPPUBus(&ppuBus);
    this->bus.bindPPU(&ppu);

}

Machine::~Machine() {
}

void Machine::reset() {
    this->cpu.reset();
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
    if(prgSize / (16 * 1024) == 1) {
        this->prg1 = new MemoryBlock(prgSize);
        for(int i = 0; i < prgSize; i++) {
            this->prg1->m_[i] = rom.PRG_[i];
        }
        bus.bindPRG_1(this->prg1);
        bus.bindPRG_2(this->prg1);
    }
    else {
        this->prg1 = new MemoryBlock(prgSize / 2);
        for(int i = 0; i < prgSize / 2; i++) {
            this->prg1->m_[i] = rom.PRG_[i];
        }
        this->prg2 = new MemoryBlock(prgSize / 2);
        for(int i = 0; i < prgSize / 2; i++) {
            this->prg2->m_[i] = rom.PRG_[prgSize / 2 + i];
        }
        bus.bindPRG_1(this->prg1);
        bus.bindPRG_2(this->prg2);
    }

    // load CHR
    u16 chrSize = rom.getCHRSize();
    // assume that it's 8kb
    this->chr1 = new MemoryBlock(chrSize / 2);
    this->chr2 = new MemoryBlock(chrSize / 2);
    for(int i = 0; i < chrSize / 2; i++) {
        this->chr1->m_[i] = rom.CHR_[i];
        this->chr2->m_[i] = rom.CHR_[i + chrSize / 2];
    }
    ppuBus.bindPatternTable(0, this->chr1);
    ppuBus.bindPatternTable(1, this->chr2);

    reset();
}

void Machine::update() {
    for(int i = 0; i < 100; i++) {
        this->cpu.execute();
    }
    this->ppu.startVBlank();
    for(int i = 0; i < 31350; i++) {
        this->cpu.execute();
    }
    this->ppu.endVBlank();
}

void Machine::get(QRgb* pixels) {
    ppu.get();
    for(int i = 0; i < 240; i++) {
        for(int j = 0; j < 256; j++) {
            pixels[256 * i + j] = qRgb(ppu.screen[i][j][0], ppu.screen[i][j][1], ppu.screen[i][j][2]);
        }
    }

    // ppu->getCHR();
    // for(int i = 0; i < 128; i++) {
    //     for(int j = 0; j < 128; j++) {
    //         pixels[128 * i + j] = qRgb(ppu->chrScreen[i][j][0], ppu->chrScreen[i][j][1], ppu->chrScreen[i][j][2]);
    //     }
    // }
}
