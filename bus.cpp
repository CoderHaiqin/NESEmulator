#include "bus.h"
#include <QDebug>
#include "cpu.h"

Bus::Bus() : ram_(nullptr) {}

void Bus::bindRAM(MemoryBlock *ram) {
    ram_ = ram;
}

void Bus::bindExtendedRAM(MemoryBlock *extendedRAM) {
    extendedRAM_ = extendedRAM;
}

void Bus::bindPPU(PPU* ppu) {
    ppu_ = ppu;
}

void Bus::bindCPU(CPU* cpu) {
    cpu_ = cpu;
}

void Bus::bindMapper(Mapper* mapper) {
    mapper_ = mapper;
}

u8 Bus::read(u16 address) {
    //qDebug() << "read: " << Qt::hex << Qt::showbase << address << Qt::endl;
    u8 result = 0;
    if(address < 0x2000) {
        result = (*ram_)[address & 0x7ff];
    } else if (address < 0x4000) {
        result = ppu_->read(address - 0x2000);
    } else if(address < 0x4020) {
        result = ioRegister_->read(address - 0x4000);
    } else if(address < 0x6000) {

    } else if(address < 0x8000) {
        if(hasExtendedRAM_) {
            result = (*extendedRAM_)[address - 0x6000];
        }
    } else {
        result = mapper_->readPRG(address);
    }

    return result;
}

void Bus::write(u16 address, u8 value) {

    //qDebug() << "write: " << Qt::hex << Qt::showbase << value << " to "<< Qt::hex << Qt::showbase << address<< Qt::endl;
    if(address < 0x2000) {
        (*ram_)[address & 0x7ff] = value;
    } else if (address < 0x4000) {
        ppu_->write(address - 0x2000, value);
    } else if(address == 0x4014) {
        u16 pageAddr = (u16)(value) << 8;
        // TODO
        pageAddr = pageAddr & 0x7ff;
        cpu_->oamCycle();
        for(int i = 0; i < 0xff; i++) {
            ppu_->ppuram[i] = (*ram_)[pageAddr + i];
        }

    }
    else if(address < 0x4020) {
        ioRegister_->write(address - 0x4000, value);
    } else if(address < 0x6000) {

    } else if(address < 0x8000) {
        if(hasExtendedRAM_) {
            (*extendedRAM_)[address - 0x6000] = value;
        }
    } else {
        mapper_->writePRG(address, value);
    }
}

void Bus::bindIORegister(IORegister* ioRegister) {
    ioRegister_ = ioRegister;
}
