#ifndef PPU_H
#define PPU_H
#include "type.h"
#include "ppubus.h"
#include "constant.h"

class CPU;

struct Control {
    u8 N;
    u8 I;
    u8 S;
    u8 B;
    u8 H;
    u8 P;
    u8 V;

    void write(u8 value) {
        N = value & 0x3;
        I = (value >> 2) & 0x1;
        S = (value >> 3) & 0x1;
        B = (value >> 4) & 0x1;
        H = (value >> 5) & 0x1;
        P = (value >> 6) & 0x1;
        V = (value >> 7) & 0x1;
    }
    u8 read() {
        u8 result = 0;
        result += N;
        result += I << 2;
        result += S << 3;
        result += B << 4;
        result += H << 5;
        result += P << 6;
        result += V << 7;
        return result;
    }
};

struct Mask {
    u8 Grey;
    u8 m;
    u8 M;
    u8 b;
    u8 s;
    u8 R;
    u8 G;
    u8 B;

    void write(u8 value) {
        Grey = value & 0x1;
        m = (value >> 1) & 0x1;
        M = (value >> 2) & 0x1;
        b = (value >> 3) & 0x1;
        s = (value >> 4) & 0x1;
        R = (value >> 5) & 0x1;
        G = (value >> 6) & 0x1;
        B = (value >> 7) & 0x1;
    }
    u8 read() {
        u8 result = 0;
        result += Grey;
        result += m << 1;
        result += M << 2;
        result += b << 3;
        result += s << 4;
        result += R << 5;
        result += G << 6;
        result += B << 7;
        return result;
    }
};

struct Status {
    u8 V;
    u8 S;
    u8 O;
    void write(u8 value) {
        V = (value >> 7) & 0x1;
        S = (value >> 6) & 0x1;
        O = (value >> 5) & 0x1;
    }
    u8 read() {
        u8 result = 0;
        result += O << 5;
        result += S << 6;
        result += V << 7;
        return result;
    }
};

class PPU
{
private:
    Control control_;
    Mask mask_;
    Status status_;
    u8 OAMAddr_ = 0;
    // u8 OAMData_;

    u8 w_register_ = 1;
    u8 xscroll = 0;

    u16 addr_tmp_ = 0;
    u16 addr_ = 0;
    u16 data_buffer = 0;
    u8 OAMDMA_ = 0;

    PPUBus* ppuBus_ = nullptr;
    CPU* cpu_;

    int scanLine_ = 0;
    int cycle_ = 0;
    bool evenFrame_ = false;

    std::vector<int> spriteInScanline_;

public:
    PPU();
    u8 screen[Constant::screenHeight][Constant::screenWidth][Constant::colorChannel];
    u8 chrScreen[128][128][3];

    u8 ppuram[0x100];

    void bindPPUBus(PPUBus* ppuBus);
    void bindCPU(CPU* cpu);

    void write(u16 addr, u8 value);
    u8 read(u16 addr);
    u8 getCtrl();

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

//    void get();

//    void getCHR();

    void execute();

    int step;
    void beforeRender();
    void render();
    void afterRender();
    void vBlank();
};

#endif // PPU_H
