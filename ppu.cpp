#include "ppu.h"
#include <iostream>
#include "cpu.h"
#include <assert.h>


PPU::PPU() {
//    line.resize(Constant::screenWidth);

    spriteInScanline_.resize(0);
    for(int i = 0; i < 0x100; i++) {
        ppuram[i] = 0;
    }
}

void PPU::bindPPUBus(PPUBus* ppuBus) {
    ppuBus_ = ppuBus;
}

void PPU::bindCPU(CPU* cpu) {
    cpu_ = cpu;
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
    }
    else {
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
    return control_.read();
}

u8 PPU::readStatus() {
    w_register_ = true;
    return status_.read();
}

u8 PPU::readOAMData() {
    u8 result = ppuBus_->read(OAMAddr_);
    OAMAddr_++;

    return result;
}

u8 PPU::readData() {

    u8 result = ppuBus_->read(addr_);

    if(control_.I) {
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
    control_.write(value);
    addr_tmp_ &= ~0xc00;
    addr_tmp_ |= (u16)(control_.read() & 0x3) << 10;
}

void PPU::writeMask(u8 value) {
    mask_.write(value);
}

void PPU::writeOAMAddr(u8 value) {
    OAMAddr_ = value;
}

void PPU::writeOAMData(u8 value) {
    ppuram[OAMAddr_] = value;
    OAMAddr_++;
}

void PPU::writeScroll(u8 value) {
    if(w_register_ == 1) {
        addr_tmp_ &= ~0x1f;
        addr_tmp_ |= (value >> 3) & 0x1f;
        xscroll = value & 0x7;
    } else {
        addr_tmp_ &= ~0x73e0;
        addr_tmp_ |= ((value & 0x7) << 12) | ((value & 0xf8) << 2);
    }

    w_register_ = (w_register_ + 1) % 2;
}

void PPU::writeAddr(u8 value) {
    if(w_register_ == 1) {
        addr_tmp_ = addr_tmp_ & ~0xff00;
        addr_tmp_ = addr_tmp_ | ((u16)(value & 0x3f) << 8);
    } else {
        addr_tmp_ = addr_tmp_ & ~0x00ff;
        addr_tmp_ = addr_tmp_ | value;
        addr_ = addr_tmp_;
    }

    w_register_ = (w_register_ + 1) % 2;
}

void PPU::writeData(u8 value) {
    ppuBus_->write(addr_, value);
    if(control_.I) {
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
    status_.V = 1;
}

void PPU::endVBlank() {
    status_.V = 0;
}

void PPU::execute() {
    switch (step) {
    case 0:
        beforeRender();
        break;
    case 1:
        render();
        break;
    case 2:
        afterRender();
        break;
    case 3 :
        vBlank();
        break;
    default:
        assert(0);
    }

    cycle_++;
}

void PPU::beforeRender() {
    if(cycle_ == 1) {
        this->endVBlank();
        status_.S = 0;
    } else if (cycle_ == 256 + 2 && mask_.b && mask_.s) {
        addr_ &= ~0x41f;
        addr_ |= addr_tmp_ & 0x41f;
    } else if (cycle_ > 280 && cycle_ <= 304 && mask_.b && mask_.s) {
        addr_ &= ~0x7be0;
        addr_ |= addr_tmp_ & 0x7be0;
    }
    if (cycle_ >= 340 - (!evenFrame_ && mask_.b && mask_.s)){
        //
        cycle_ = 0;
        scanLine_ = 0;
        step = 1;
    }

    if(cycle_ == 260 && mask_.b && mask_.s) {
        // IRQ
    }
}

void PPU::render() {
    if(cycle_ > 0 && cycle_ <= 256) {
        int x = cycle_ - 1;
        int y = scanLine_;

        u8 bgColor = 0;
        u8 sprColor = 0;
        bool bgOpaque = false;
        bool sprOpaque = true;
        bool spriteForeground = false;
        // u8 bgPaletteIndex = 0;
        if(x == 58 && y == 33) {
            int a = 0;
        }

        if(mask_.b) {

            int x1 = (x + xscroll) % 8;
            if(x >= 8 || mask_.m) {
                u16 addrInNameTable = 0x2000 + (addr_ & 0xfff);
                u8 indexInPatternTable = ppuBus_->read(addrInNameTable);
                u16 addrInPatternTable = (16 * indexInPatternTable) + ((addr_ >> 12) & 0x7);
                if(control_.B) {
                    addrInPatternTable += 0x1000;
                }
                u8 tileLeft = ppuBus_->read(addrInPatternTable);
                u8 tileRight = ppuBus_->read(addrInPatternTable + 8);

                // low on the left
                u8 leftBit = (tileLeft & (1 << (7 - x1))) != 0;
                u8 rightBit = (tileRight & (1 << (7 - x1))) != 0;
                u8 combined = leftBit + (rightBit << 1);
                bgOpaque = combined;
                u16 addrInAttrTable = 0x23C0 | (addr_ & 0x0C00) | ((addr_ >> 4) & 0x38)
                       | ((addr_ >> 2) & 0x07);
                u8 attr = ppuBus_->read(addrInAttrTable);
                int shift = ((addr_ >> 4) & 4) | (addr_ & 2);
                //Extract and set the upper two bits for the color
                attr = (attr >> shift) & 0x3;
                // bgPaletteIndex = ppuBus_->read(0x3f00 + 4 * attr + combined);
                bgColor += attr * 4 + combined;
            }
            if(x1 == 7) {
                if ((addr_ & 0x001F) == 31) {
                    addr_ &= ~0x001F;
                    addr_ ^= 0x0400;
                } else {
                    addr_ += 1;
                }
            }
        }

        if(mask_.s && (mask_.M || x >= 8)) {
            for(auto i : spriteInScanline_) {
                u8 spriteX = ppuram[4 * i + 3];
                if(x < spriteX || x >= spriteX + 8) {
                    continue;
                }

                u8 spriteY = ppuram[4 * i] + 1;
                u8 tile = ppuram[4 * i + 1];
                u8 attr = ppuram[i * 4 + 2];

                int length = 8 + 8 * (control_.H);
                u8 x1 = x - spriteX;
                u8 y1 = (y - spriteY) % length;
                if ((attr & 0x40) == 0) {
                    x1 = 7 - x1;
                }
                if ((attr & 0x80) != 0) {
                    y1 = length - 1 - y1;
                }

                u16 addrInPatternTable = 0;
                if (length == 8) {
                    addrInPatternTable = tile * 16 + y1;
                    if(control_.S) {
                        addrInPatternTable += 0x1000;
                    }
                } else {
                    y1 = (y1 & 7) | ((y1 & 8) << 1);
                    addrInPatternTable = (tile >> 1) * 32 + y1;
                    addrInPatternTable |= (tile & 1) << 12;
                }

                sprColor += (ppuBus_->read(addrInPatternTable) >> (x1)) & 1;
                sprColor += ((ppuBus_->read(addrInPatternTable + 8) >> (x1)) & 1) << 1;

                sprOpaque = sprColor;
                if (!sprOpaque) {
                    continue;
                }
                sprColor += 0x10;
                sprColor += (attr & 0x3) << 2;

                spriteForeground = !(attr & 0x20);

                if (!(status_.S) && (mask_.b) && i == 0 && sprOpaque && bgOpaque) {
                    status_.S = true;
                }

                break;

            }
        }
        u8 paletteAddr = 0;
        if ((!bgOpaque && sprOpaque) ||
            (bgOpaque && sprOpaque && spriteForeground)) {
            paletteAddr = sprColor;

        } else if (!bgOpaque && !sprOpaque) {
            paletteAddr = 0;
        } else {
            paletteAddr = bgColor;
        }

        u8 paletteIndex = ppuBus_->read(0x3f00 + paletteAddr);
        if(y % 2 == 0 && paletteAddr != 0) {
            int a = 0;
        }

        this->screen[y][x][0] = Constant::palette[paletteIndex][0];
        this->screen[y][x][1] = Constant::palette[paletteIndex][1];
        this->screen[y][x][2] = Constant::palette[paletteIndex][2];

//        this->line[x] = Constant::palette[paletteIndex][0];

    }
    else if(cycle_ == 257 && mask_.b) {
        if ((addr_ & 0x7000) != 0x7000) {
            addr_ += 0x1000;
        } else {
            addr_ &= ~0x7000;
            int y = (addr_ & 0x03E0) >> 5;
            if (y == 29) {
                y = 0;
                addr_ ^= 0x0800;
            } else if (y == 31) {
                y = 0;
            } else {
                y += 1;
            }
            addr_ = (addr_ & ~0x03E0) | (y << 5);
        }
    } else if(cycle_ == 258 && (mask_.b) && (mask_.s)) {
        addr_ &= ~0x41f;
        addr_ |= addr_tmp_ & 0x41f;
    }
    if (cycle_ >= 340){

        spriteInScanline_.resize(0);
        bool spriteOverflow = false;
        //
        int length = 8 + 8 * (control_.H);

        for (int i = OAMAddr_ / 4; i <= 63; i++){
            if (ppuram[i * 4] > scanLine_ - length && ppuram[i * 4] <= scanLine_){
                if (spriteInScanline_.size() == 8){
                    spriteOverflow = true;
                    break;
                }else{
                    spriteInScanline_.push_back(i);
                }
            }
        }
        status_.O = spriteOverflow;
        scanLine_++;
        cycle_ = 0;
    }
    if(scanLine_ >= 240) {
        step = 2;
    }
}

void PPU::afterRender() {
    if(cycle_ >= 340) {
        scanLine_++;
        cycle_ = 0;
        step = 3;


//        bool flag = true;
//        for(int i = 0; i < Constant::screenWidth; i++) {
//            if(line[i] != 0) {
//                flag = false;
//            }
//        }
//        if(flag) {
//            int a = 0;
//        }
    }
}

void PPU::vBlank() {
    if(cycle_ == 1 && scanLine_ == 241) {
        startVBlank();
        if(control_.V) {
            cpu_->nmi();
        }
    }

    if(cycle_ >= 340) {
        scanLine_++;
        cycle_ = 0;
    }

    if(scanLine_ >= 261) {
        step = 0;
        scanLine_ = 0;
        evenFrame_ = !evenFrame_;
    }
}

void PPU::getCHR() {
    for(int i = 0; i < 32; i++) {
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

                    int offset = 4 * 11;
//                    this->chrScreen[i * 8 + x][j * 8 + y][0] = Constant::palette[offset + combined][0];
//                    this->chrScreen[i * 8 + x][j * 8 + y][1] = Constant::palette[offset + combined][1];
//                    this->chrScreen[i * 8 + x][j * 8 + y][2] = Constant::palette[offset + combined][2];
                }
            }
        }
    }
}

void PPU::get() {
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
            attr = (attr >> offset) & 0x3;

            // 2. handle pattern table
            int indexInNameTable = i * 32 + j;
            int indexInPatternTable = ppuBus_->read(indexInNameTable + 0x2000);

            // std::cout << i << ' ' << j << ' ' << indexInNameTable << ' ' << indexInPatternTable << std::endl;

            for(int x = 0; x < 8; x++) {
                u16 addrInPatternTable = indexInPatternTable * 16;

                addrInPatternTable += 0x1000;

                u8 tileLeft = ppuBus_->read(addrInPatternTable + x);
                u8 tileRight = ppuBus_->read(addrInPatternTable + 8 + x);

                // std::cout << (int)tileLeft << " " << (int)tileRight << std::endl;
                for(int y = 0; y < 8; y++) {
                    // low on the left
                    u8 leftBit = (tileLeft >> (7 - y)) & 1;
                    u8 rightBit = (tileRight >> (7 - y)) & 1;
                    u8 combined = leftBit + (rightBit << 1);
                    u8 paletteIndex = ppuBus_->read(0x3f00 + 4 * attr + combined);


                    this->screen[i * 8 + x][j * 8 + y][0] = Constant::palette[paletteIndex][0];
                    this->screen[i * 8 + x][j * 8 + y][1] = Constant::palette[paletteIndex][1];
                    this->screen[i * 8 + x][j * 8 + y][2] = Constant::palette[paletteIndex][2];
                }
            }
        }
    }
}

void PPU::getNameTable() {
    for(int k = 0; k < 4; k++) {
    for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 32; j++) {
            // 1. handle attribution table
            u16 indexInAttrTable = 0x400*k+960 + (i / 4 * 8 + j / 4);

            u8 attr = ppuBus_->ppuMemory_.nameTable[indexInAttrTable];
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
            attr = (attr >> offset) & 0x3;

            // 2. handle pattern table
            int indexInNameTable = 0x400*k+i * 32 + j;
            int indexInPatternTable = ppuBus_->ppuMemory_.nameTable[(indexInNameTable)];

            // std::cout << i << ' ' << j << ' ' << indexInNameTable << ' ' << indexInPatternTable << std::endl;

            for(int x = 0; x < 8; x++) {
                u16 addrInPatternTable = indexInPatternTable * 16;

                addrInPatternTable += 0x1000;

                u8 tileLeft = ppuBus_->read(addrInPatternTable + x);
                u8 tileRight = ppuBus_->read(addrInPatternTable + 8 + x);

                // std::cout << (int)tileLeft << " " << (int)tileRight << std::endl;
                for(int y = 0; y < 8; y++) {
                    // low on the left
                    u8 leftBit = (tileLeft >> (7 - y)) & 1;
                    u8 rightBit = (tileRight >> (7 - y)) & 1;
                    u8 combined = leftBit + (rightBit << 1);
                    u8 paletteIndex = ppuBus_->read(0x3f00 + 4 * attr + combined);

                    int offsetx = (k / 2) * 240;
                    int offsety = (k % 2) * 256;
                    this->nameTableScreen[i * 8 + x + offsetx][j * 8 + y + offsety][0] = Constant::palette[paletteIndex][0];
                    this->nameTableScreen[i * 8 + x + offsetx][j * 8 + y + offsety][1] = Constant::palette[paletteIndex][1];
                    this->nameTableScreen[i * 8 + x + offsetx][j * 8 + y + offsety][2] = Constant::palette[paletteIndex][2];
                }
            }
        }
    }
    }
}

