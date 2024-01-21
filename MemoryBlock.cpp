#include "MemoryBlock.h"

MemoryBlock::MemoryBlock(u16 length) {
    m_ = new u8[length];
    length_ = length;
}

MemoryBlock::~MemoryBlock() {
    delete [] m_;
}

void MemoryBlock::write(u16 addr, u8 value) {
    if(addr < length_) {
        m_[addr] = value;
    }
    else {
        return;
    }
}

u8 MemoryBlock::read(u16 addr) {
    if(addr < length_) {
        return m_[addr];
    }

    return 0;
}
