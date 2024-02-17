#ifndef MAPPERNROM_H
#define MAPPERNROM_H
#include "mapper.h"

class MapperNROM : public Mapper
{
public:
    MemoryBlock ramForCHR;
    bool twoPRGPage;
    bool useRAMforCHR;
    MapperNROM(ROM* rom);

    u8 readPRG(u16 address) override;
    void writePRG(u16 address, u8 value) override;
    u8 readCHR(u16 address) override;
    void writeCHR(u16 address, u8 value) override;

};

#endif // MAPPERNROM_H
