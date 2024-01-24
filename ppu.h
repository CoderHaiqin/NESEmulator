#ifndef PPU_H
#define PPU_H
#include "type.h"
#include "ppubus.h"
#include "constant.h"

class PPU
{
private:
    u8 control_ = 0;
    u8 mask_ = 0;
    u8 status_ = 0;
    u8 OAMAddr_ = 0;
    // u8 OAMData_;

    u8 w_register_ = 0;
    u8 scroll_[2] = {0};
    u16 addr_ = 0;
    u8 OAMDMA_ = 0;

    PPUBus* ppuBus_ = nullptr;

    int scanLine = 0;
    int cycle = 0;

public:
    PPU();
    u8 screen[Constant::screenWidth][Constant::screenHeight][Constant::colorChannel];
    u8 chrScreen[128][128][3];

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

    void reset();
    void update();

    void startVBlank();
    void endVBlank();

    void get();

    void getCHR();
};

#endif // PPU_H
