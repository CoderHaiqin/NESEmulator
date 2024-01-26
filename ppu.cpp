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
    w_register_ = true;
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

    if (addr_ < 0x3f00){
        uint8_t tmp = data_buffer;
        data_buffer = result;
        return tmp;
    }else{
        //data_buffer = p_bus->ram_data[data_addr.data & 0x3ff];
        data_buffer = result;
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
    ppuram[OAMAddr_] = value;
    OAMAddr_++;
}

void PPU::writeScroll(u8 value) {
    scroll_[w_register_] = value;
    w_register_ = (w_register_ + 1) % 2;
}

void PPU::writeAddr(u8 value) {
    if(w_register_ == 1) {
        addr_tmp_ = addr_tmp_ & 0x00ff;
        addr_tmp_ = addr_tmp_ | ((u16)(value & 0x3f) << 8);
    } else {
        addr_tmp_ = addr_tmp_ & 0x3f00;
        addr_tmp_ = addr_tmp_ | value;
        addr_ = addr_tmp_;
    }
    // if(addr_ == 8187) {
    //     int a = 0;
    // }
    // std::cout << (int)value << ' ' << addr_ << std::endl;

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
            u16 indexInAttrTable = 960 + (i / 4 * 8 + j / 4);
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
            // std::cout << i << ' ' << j << ' ' << indexInNameTable << ' ' << indexInPatternTable << std::endl;

            for(int x = 0; x < 8; x++) {
                u16 addrInPatternTable = indexInPatternTable * 16;
                if(control_ & (1 << 4)) {
                    addrInPatternTable += 0x1000;
                }
                u8 tileLeft = ppuBus_->read(addrInPatternTable + x);
                u8 tileRight = ppuBus_->read(addrInPatternTable + 8 + x);

                // std::cout << (int)tileLeft << " " << (int)tileRight << std::endl;
                for(int y = 0; y < 8; y++) {
                    // low on the left
                    u8 leftBit = (tileLeft & (1 << (7 - y))) != 0;
                    u8 rightBit = (tileRight & (1 << (7 - y))) != 0;
                    u8 combined = leftBit + (rightBit << 1);
                    u8 paletteIndex = ppuBus_->read(0x3f00 + 4 * attr + combined);

                    // if(t != paletteIndex) {
                    //     std::cout << (int)t << std::endl;
                    // }
                    // t = paletteIndex;

                    this->screen[i * 8 + x][j * 8 + y][0] = Constant::palette[paletteIndex][0];
                    this->screen[i * 8 + x][j * 8 + y][1] = Constant::palette[paletteIndex][1];
                    this->screen[i * 8 + x][j * 8 + y][2] = Constant::palette[paletteIndex][2];
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
                    u8 leftBit = (tileLeft & (1 << (7 - y))) != 0;
                    u8 rightBit = (tileRight & (1 << (7 - y))) != 0;
                    u8 combined = leftBit + (rightBit << 1);

                    int offset = 4 * 12;
                    this->chrScreen[i * 8 + x][j * 8 + y][0] = Constant::palette[offset + combined][0];
                    this->chrScreen[i * 8 + x][j * 8 + y][1] = Constant::palette[offset + combined][1];
                    this->chrScreen[i * 8 + x][j * 8 + y][2] = Constant::palette[offset + combined][2];
                }
            }
        }
    }
}
