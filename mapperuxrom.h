#ifndef MAPPERUXROM_H
#define MAPPERUXROM_H
#include "mapper.h"

class MapperUxROM : public Mapper
{
public:
    MapperUxROM(ROM* rom);

    u8 readPRG(u16 address) override;
    void writePRG(u16 address, u8 value) override;
    u8 readCHR(u16 address) override;
    void writeCHR(u16 address, u8 value) override;
};

#endif // MAPPERUXROM_H
