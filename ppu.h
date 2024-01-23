#ifndef PPU_H
#define PPU_H
#include "type.h"
#include "ppubus.h"

class PPU
{
public:
    PPU();

    u8 control_;
    u8 mask_;
    u8 status_;
    u8 OAMAddr_;
    // u8 OAMData_;

    u8 w_register_;
    u8 scroll_[2];
    u16 addr_;
    u8 OAMDMA_;

    PPUBus* ppuBus_;

    void bindPPUBus(PPUBus* ppuBus);

    void write(u16 addr, u8 value);
    u8 read(u16 addr);

    u8 readStatus();
    u8 readOAMData();
    u8 readData();

    void writeControl(u8 value);
    void writeMask(u8 value);
    void writeStatus(u8 value);
    void writeOAMAddr(u8 value);
    void writeOAMData(u8 value);
    void writeScroll(u8 value);
    void writeAddr(u8 value);
    void writeData(u8 value);
};

#endif // PPU_H
