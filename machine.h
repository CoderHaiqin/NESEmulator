#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "ppu.h"
#include "qrgb.h"
#include "ioregister.h"

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

    IORegister ioRegister;
    bool keyState[0x10];

public:
    Machine();
    ~Machine();

    void reset();
    void load(const std::string& path);
    void update();

    void get(QRgb* pixels);

    void input(int key, bool flag);
};

#endif // MACHINE_H
