#include "ppubus.h"
#include <assert.h>
#include <QDebug>

PPUBus::PPUBus() {}

void PPUBus::write(u16 addr, u8 value) {
    static std::list<u16> lastaddr(0);
    lastaddr.push_back(addr);
    if(lastaddr.size() > 100) {
        lastaddr.pop_front();
    }
//    if(addr == 0x2087 || addr == 0x2487) {
//        qDebug() << Qt::hex << Qt::showbase << value << Qt::endl;
//        int a = 0;
//    }
    // qDebug() << "PPU write: " << Qt::hex << Qt::showbase << addr << " " << value << Qt::endl;
    if(addr < 0x2000) {
        // assert(0);
        mapper_->writeCHR(addr, value);
    } else if (addr < 0x2400) {
        ppuMemory_.nameTable[addr - 0x2000 + ppuMemory_.nameTable0] = value;
    } else if (addr < 0x2800) {
        ppuMemory_.nameTable[addr - 0x2400 + ppuMemory_.nameTable1] = value;
    } else if (addr < 0x2c00) {
        ppuMemory_.nameTable[addr - 0x2800 + ppuMemory_.nameTable2] = value;
    } else if (addr < 0x3000) {
        ppuMemory_.nameTable[addr - 0x2c00 + ppuMemory_.nameTable3] = value;
    } else if (addr < 0x3f00) {
        write(addr - 0x1000, value);
    } else {
        addr -= 0x3f00;
        addr = addr & 0x1f;
        if (!(addr & (u16)0x03)) {
            addr = addr & (~(0x10));
        }
        ppuMemory_.PaletteRAMIndex[addr] = value;

    }
}

u8 PPUBus::read(u16 addr) {
    if(addr < 0x2000) {
        return mapper_->readCHR(addr);
    } else if (addr < 0x2400) {
        return ppuMemory_.nameTable[addr - 0x2000 + ppuMemory_.nameTable0];
    } else if (addr < 0x2800) {
        return ppuMemory_.nameTable[addr - 0x2400 + ppuMemory_.nameTable1];
    } else if (addr < 0x2c00) {
        return ppuMemory_.nameTable[addr - 0x2800 + ppuMemory_.nameTable2];
    } else if (addr < 0x3000) {
        return ppuMemory_.nameTable[addr - 0x2c00 + ppuMemory_.nameTable3];
    } else if (addr < 0x3f00) {
        return read(addr - 0x1000);
    } else {
        addr -= 0x3f00;
        addr = addr & 0x1f;
        if (!(addr & (u16)0x03)) {
            addr = addr & (~(0xc));
        }
        return ppuMemory_.PaletteRAMIndex[addr];
    }

    return 0;
}

void PPUBus::bindMapper(Mapper *mapper) {
    mapper_ = mapper;
}

void PPUBus::updateMirror() {
    u8 mirror = mapper_->getNameTableMirror();
    static u16 m[3][4] = {
        {0, 0, 0x800, 0x800},
        {0, 0x400, 0, 0x400},
        {0, 0x400, 0x800, 0xc00},
        };

    ppuMemory_.nameTable0 = m[mirror][0];
    ppuMemory_.nameTable1 = m[mirror][1];
    ppuMemory_.nameTable2 = m[mirror][2];
    ppuMemory_.nameTable3 = m[mirror][3];
}
