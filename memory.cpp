#include "memory.h"
#include "constant.h"
#include <assert.h>

Memory::Memory()
{
    this->memory_ = new u8[Constant::memorySize];
}

Memory::~Memory()
{
    delete [](this->memory_);
}

u8 Memory::read(u16 address) {
    if(address >= Constant::memorySize) {
        assert(false);
    }

    return this->memory_[address];
}

void Memory::write(u16 address, u8 value) {
    if(address >= Constant::memorySize) {
        assert(false);
    }

    this->memory_[address] = value;
}
