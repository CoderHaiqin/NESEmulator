#include "mapperuxrom.h"

MapperUxROM::MapperUxROM(ROM* rom)
{

}

u8 MapperUxROM::readPRG(u16 address) {
    return 0;
}

void MapperUxROM::writePRG(u16 address, u8 value) {
    return;
}

u8 MapperUxROM::readCHR(u16 address) {
    return 0;
}

void MapperUxROM::writeCHR(u16 address, u8 value) {
    return;
}
