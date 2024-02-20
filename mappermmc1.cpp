#include "mappermmc1.h"
#include "assert.h"

#include "ppubus.h"

MapperMMC1::MapperMMC1(ROM* rom)
{
    if(rom->getCHRSize() == 0) {
        useRAMforCHR = true;
        ramForCHR.resize(0x2000);
    } else {
        useRAMforCHR = false;
        CHRPagePos0 = 0;
        CHRPagePos1 = 0x1000 * CHRRegister1;
    }

    PRGPagePos0 = 0;
    PRGPagePos1 = rom->PRG_.size() - 0x4000;
}

u8 MapperMMC1::readPRG(u16 address) {
    address -= 0x8000;
    if(address < 0x4000) {
        return rom->PRG_[PRGPagePos0 + address];
    } else {
        return rom->PRG_[PRGPagePos1 + address % 0x4000];
    }
}

void MapperMMC1::writePRG(u16 address, u8 value) {
    if(value & 0x80) {
        tmpRegister = 0;
        writeCount = 0;
        PRGmode = 3;
        updatePRGPosition();

        return;
    }

    tmpRegister = (tmpRegister >> 1) | ((value & 1) << 4);
    writeCount++;

    if(writeCount != 5) {
        return;
    }

    if (address <= 0x9fff) {
        int t[4] = {3, 4, 0, 1};
        mirror = t[tmpRegister & 0x3];

        ppuBus->updateMirror();

        CHRmode = (tmpRegister & 0x10) >> 4;
        PRGmode = (tmpRegister & 0xc) >> 2;
        updatePRGPosition();

        if (CHRmode == 0) {
            CHRPagePos0 = 0x1000 * (CHRRegister0 | 1); //ignore last bit
            CHRPagePos1 = CHRPagePos0 + 0x1000;
        }
        else {
            CHRPagePos0 = 0x1000 * CHRRegister0;
            CHRPagePos1 = 0x1000 * CHRRegister1;
        }
    }
    else if (address <= 0xbfff) {
        CHRRegister0 = tmpRegister;
        CHRPagePos0 = 0x1000 * (tmpRegister | (1 - CHRmode)); //OR 1 if 8KB mode
        if (CHRmode == 0)
            CHRPagePos1 = CHRPagePos0 + 0x1000;
    }
    else if (address <= 0xdfff) {
        CHRRegister1 = tmpRegister;
        if(CHRmode == 1)
            CHRPagePos1 = 0x1000 * tmpRegister;
    }
    else {
        tmpRegister &= 0xf;
        PRGRegister = tmpRegister;
        updatePRGPosition();
    }

    tmpRegister = 0;
    writeCount = 0;

}

u8 MapperMMC1::readCHR(u16 address) {
    assert(address < 0x2000);
    if (useRAMforCHR) {
        return ramForCHR[address];
    } else if(address < 0x1000) {
        return rom->CHR_[CHRPagePos0 + address];
    } else {
        return rom->CHR_[CHRPagePos1 + address % 0x1000];
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

u8 MapperMMC1::getNameTableMirror() {
    return mirror;
}

void MapperMMC1::updatePRGPosition() {
    if(PRGmode == 0 || PRGmode == 1) {
        PRGPagePos0 = 0x4000 * (PRGRegister & ~1);
        PRGPagePos1 = PRGPagePos0 + 0x4000;
    } else if(PRGmode == 2) {
        PRGPagePos0 = 0;
        PRGPagePos1 = PRGPagePos0 + 0x4000 * PRGRegister;
    } else {
        PRGPagePos0 = 0x4000 * PRGRegister;
        PRGPagePos1 = rom->PRG_.size() - 0x4000;
    }
}
