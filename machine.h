#ifndef MACHINE_H
#define MACHINE_H

#include "cpu.h"
#include "ppu.h"
#include "qrgb.h"
#include "ioregister.h"
#include "mapper.h"

class Machine
{
private:
    CPU cpu;
    Bus bus;
    ROM rom;
    MemoryBlock* ram;
    MemoryBlock* extended_ram;

    PPU ppu;
    PPUBus ppuBus;

    Mapper* mapper;

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
