#include "mappermmc1.h"
#include "assert.h"

MapperMMC1::MapperMMC1(ROM* rom)
{
    if(rom->getPRGSize() == 16 * 1024) {
        twoPRGPage = false;
    } else if(rom->getPRGSize() == 2 * 16 * 1024) {
        twoPRGPage = true;
    } else {
        assert(0);
    }

    if(rom->getCHRSize() == 0) {
        useRAMforCHR = true;
        ramForCHR.resize(0x2000);
    } else {
        useRAMforCHR = false;
    }
}

u8 MapperMMC1::readPRG(u16 address) {
    address -= 0x8000;
    if(twoPRGPage) {
        return rom->PRG_[address];
    } else {
        return rom->PRG_[address % 0x4000];
    }
}

void MapperMMC1::writePRG(u16 address, u8 value) {
    address -= 0x8000;
    if(twoPRGPage) {
        rom->PRG_[address] = value;
    } else {
        rom->PRG_[address % 0x4000] = value;
    }
}

u8 MapperMMC1::readCHR(u16 address) {
    assert(address < 0x2000);
    if(useRAMforCHR) {
        return ramForCHR[address];
    } else {
        return rom->CHR_[address];
    }
}

void MapperMMC1::writeCHR(u16 address, u8 value) {
    assert(address);
    if(useRAMforCHR) {
        ramForCHR[address] = value;
    } else {
        rom->CHR_[address] = value;
    }
}
