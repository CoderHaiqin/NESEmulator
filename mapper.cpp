#include "mapper.h"
#include "mappernrom.h"
#include "mapperuxrom.h"
#include "mappermmc1.h"
#include <iostream>

Mapper::Mapper()
{

}

Mapper* Mapper::createFromROM(ROM *rom) {
    Mapper* result = nullptr;

    switch(rom->mapperType_) {
    case 0:
        result = new MapperNROM(rom);
        break;
    case 1:
        result = new MapperMMC1(rom);
        break;
    case 2:
        result = new MapperUxROM(rom);
        break;
    default:
        std::cout << "mapper not implemented" << std::endl;
    }

    if(result != nullptr) {
        result->rom = rom;
    }
    return result;
}

u8 Mapper::getNameTableMirror() {
    return rom->getNameTableMirror();
}
