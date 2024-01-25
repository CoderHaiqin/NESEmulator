#include "ppu.h"
#include <iostream>

PPU::PPU() {}

void PPU::bindPPUBus(PPUBus* ppuBus) {
    ppuBus_ = ppuBus;
}

void PPU::write(u16 address, u8 value) {
    if(address == 0) {
        writeControl(value);
    } else if(address == 1) {
        writeMask(value);
    } else if(address == 2) {
        std::cout << "invalid write to status" << std::endl;
    } else if(address == 3) {
        writeOAMAddr(value);
    } else if(address == 4) {
        writeOAMData(value);
    } else if(address == 5) {
        writeScroll(value);
    } else if(address == 6) {
        writeAddr(value);
    } else if(address == 7) {
        writeData(value);
    } else {
        std::cout << "undefine write to PPU" << std::endl;
    }
}

u8 PPU::read(u16 address) {
    if(address == 0) {
        std::cout << "invalid read on control" << std::endl;
    } else if(address == 1) {
        std::cout << "invalid read on mask" << std::endl;
    } else if(address == 2) {
        return readStatus();
    } else if(address == 3) {
        std::cout << "invalid read on OAMAddr" << std::endl;
    } else if(address == 4) {
        return readOAMData();
    } else if(address == 5) {
        std::cout << "invalid read on scroll" << std::endl;
    } else if(address == 6) {
        std::cout << "invalid read on addr" << std::endl;
    } else if(address == 7) {
        return readData();
    } else {
        std::cout << "undefine write to PPU" << std::endl;
    }

    return 0;
}

u8 PPU::getCtrl() {
    return control_;
}

u8 PPU::readStatus() {
    return status_;
}

u8 PPU::readOAMData() {
    u8 result = ppuBus_->read(OAMAddr_);
    OAMAddr_++;

    return result;
}

u8 PPU::readData() {
    u8 result = ppuBus_->read(addr_);

    if(control_ & 0x4) {
        addr_ += 32;
    } else {
        addr_ += 1;
    }

    return result;
}

void PPU::writeControl(u8 value) {
    control_ = value;
}

void PPU::writeMask(u8 value) {
    mask_ = value;
}

void PPU::writeOAMAddr(u8 value) {
    OAMAddr_ = value;
}

void PPU::writeOAMData(u8 value) {
    ppuBus_->write(OAMAddr_, value);
    OAMAddr_++;
}

void PPU::writeScroll(u8 value) {
    scroll_[w_register_] = value;
    w_register_ = (w_register_ + 1) % 2;
}

void PPU::writeAddr(u8 value) {
    if(w_register_ == 1) {
        addr_ = addr_ & 0x3f00;
        addr_ = addr_ | value;
    } else {
        addr_ = addr_ & 0x00ff;
        addr_ = addr_ | ((u16)(value & 0x3f) << 8);
    }

    w_register_ = (w_register_ + 1) % 2;
}

void PPU::writeData(u8 value) {
    ppuBus_->write(addr_, value);
    if(control_ & 0x4) {
        addr_ += 32;
    } else {
        addr_ += 1;
    }
}

void PPU::reset() {

}

void PPU::update() {

}

void PPU::startVBlank() {
    this->status_ = this->status_ | 0x80;
}

void PPU::endVBlank() {
    this->status_ = this->status_ & 0x7f;
}

void PPU::get() {
    // find the attribute table for the 4x4 group

    for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 32; j++) {
            // 1. handle attribution table
            int indexInAttrTable = 960 + (i / 4 * 8 + j / 4);
            u8 attr = ppuBus_->read(indexInAttrTable + 0x2000);
            u8 offset = 0;
            if(i % 4 / 2 == 0 && j % 4 / 2 == 0) {
                offset = 0;
            } else if (i % 4 / 2 == 0 && j % 4 / 2 == 1) {
                offset = 2;
            } else if (i % 4 / 2 == 1 && j % 4 / 2 == 0) {
                offset = 4;
            } else {
                offset = 6;
            }
            attr = (attr >> offset) & 0xff;

            // 2. handle pattern table
            int indexInNameTable = i * 32 + j;
            int indexInPatternTable = ppuBus_->read(indexInNameTable + 0x2000);
            // std::cout << i << ' ' << j << ' ' << indexInPatternTable << std::endl;

            for(int x = 0; x < 8; x++) {
                u8 tileLeft = ppuBus_->read(indexInPatternTable * 16 + x);
                u8 tileRight = ppuBus_->read(indexInPatternTable * 16 + 8 + x);
                for(int y = 0; y < 8; y++) {
                    // low on the left
                    u8 leftBit = (tileLeft & (1 << (7 - y))) != 0;
                    u8 rightBit = (tileRight & (1 << (7 - y))) != 0;
                    u8 combined = leftBit + (rightBit << 1);


                    this->screen[i * 8 + x][j * 8 + y][0] = Constant::palette[4 * attr + combined][0];
                    this->screen[i * 8 + x][j * 8 + y][1] = Constant::palette[4 * attr + combined][1];
                    this->screen[i * 8 + x][j * 8 + y][2] = Constant::palette[4 * attr + combined][2];
                }
            }
        }
    }
}

void PPU::getCHR() {
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {

            int indexInPatternTable = 16 * i + j;

            for(int x = 0; x < 8; x++) {
                u8 tileLeft = ppuBus_->read(indexInPatternTable * 16 + x);
                u8 tileRight = ppuBus_->read(indexInPatternTable * 16 + 8 + x);
                for(int y = 0; y < 8; y++) {
                    // low on the left
                    u8 leftBit = (tileLeft & (1 << y)) != 0;
                    u8 rightBit = (tileRight & (1 << y)) != 0;
                    u8 combined = leftBit + (rightBit << 1);

                    // this->chrScreen[i * 8 + x][j * 8 + y][0] = Constant::palette[0 + combined][0];
                    // this->chrScreen[i * 8 + x][j * 8 + y][1] = Constant::palette[0 + combined][1];
                    // this->chrScreen[i * 8 + x][j * 8 + y][2] = Constant::palette[0 + combined][2];
                }
            }
        }
    }
}
