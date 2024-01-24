#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "ppu.h"
#include "qrgb.h"

class Machine
{
private:
    CPU cpu;
    Bus bus;
    MemoryBlock* ram;
    MemoryBlock* prg1;
    MemoryBlock* prg2;

    MemoryBlock* chr1;
    MemoryBlock* chr2;
    PPU ppu;
    PPUBus ppuBus;

public:
    Machine();
    ~Machine();

    void reset();
    void load(const std::string& path);
    void update();

    void get(QRgb* pixels);
};

#endif // MACHINE_H
