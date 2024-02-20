#ifndef MAPPERMMC1_H
#define MAPPERMMC1_H
#include "mapper.h"

class MapperMMC1 : public Mapper
{
public:
    MapperMMC1(ROM* rom);
    MemoryBlock ramForCHR;
    bool twoPRGPage;
    bool useRAMforCHR;

    u8 readPRG(u16 address) override;
    void writePRG(u16 address, u8 value) override;
    u8 readCHR(u16 address) override;
    void writeCHR(u16 address, u8 value) override;

};

#endif // MAPPERMMC1_H
