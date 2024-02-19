#include "mapperuxrom.h"
#include "assert.h"

MapperUxROM::MapperUxROM(ROM* rom)
{
    if(rom->getCHRSize() == 0) {
        useRAMforCHR = true;
        ramForCHR.resize(0x2000);
    } else {
        useRAMforCHR = false;
    }

    PRGPosition_ = 0;
}

u8 MapperUxROM::readPRG(u16 address) {
    address -= 0x8000;
    if (address < 0x4000) {
        return rom->PRG_[(address & 0x3fff) | (PRGPosition_ << 14)];
    }
    else {
        return rom->PRG_[rom->PRG_.size() - 0x4000 + (address & 0x3fff)];
    }
}

void MapperUxROM::writePRG(u16, u8 value) {
    PRGPosition_ = value;
}

u8 MapperUxROM::readCHR(u16 address) {
    assert(address < 0x2000);
    if(useRAMforCHR) {
        if(address > 0x1000) {
            int a = 0;
        }
        return ramForCHR[address];
    } else {
        return rom->CHR_[address];
    }
}

void MapperUxROM::writeCHR(u16 address, u8 value) {

    if(useRAMforCHR) {
        ramForCHR[address] = value;
    } else {
        rom->CHR_[address] = value;
    }
}
