#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H
#include "mapper.h"

class MapperMMC1 : public Mapper
{
public:
    MapperMMC1(ROM* rom);
    MemoryBlock ramForCHR;

    bool useRAMforCHR = 0;
    u8 PRGRegister = 0;
    u8 CHRRegister0 = 0;
    u8 CHRRegister1 = 0;

    u16 CHRPagePos0 = 0;
    u16 CHRPagePos1 = 0;

    u16 PRGPagePos0 = 0;
    u16 PRGPagePos1 = 0;

    u8 mirror = 0;

    u8 tmpRegister = 0;
    u8 writeCount = 0;
    int PRGmode = 3;
    int CHRmode = 0;

    void updatePRGPosition();

    u8 readPRG(u16 address) override;
    void writePRG(u16 address, u8 value) override;
    u8 readCHR(u16 address) override;
    void writeCHR(u16 address, u8 value) override;

    u8 getNameTableMirror() override;

};

#endif // MAPPERMMC1_H
