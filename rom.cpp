#include "rom.h"
#include <constant.h>
#include <ios>
#include <fstream>
#include <iostream>

ROM::ROM() {}

bool ROM::load(const std::string& path) {
    std::ifstream file;
    ROM rom;
    file.open(path.c_str(), std::ios::binary);
    if(!file) {
        return false;
    }
    std::vector<u8> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::cout << (int)buffer[0x400c] << std::endl;
    file.close();

    int p = 0;
    for(; p < Constant::headerLength; p++) {
        header_.push_back(buffer[p]);
    }

    if(!check()) {
        return false;
    }

    u32 PRGSize = getPRGSize();
    u32 CHRSize = getCHRSize();
    PRG_.resize(PRGSize);
    CHR_.resize(CHRSize);
    if(hasTrainerArea()) {
        p += 512;
    }

    for(int i = 0; i < PRGSize; i++) {
        PRG_[i] = buffer[p];
        p++;
    }

    for(int i = 0; i < CHRSize; i++) {
        CHR_[i] = buffer[p];
        p++;
    }

    return true;
}

bool ROM::check() {
    return header_[0] == 'N' && header_[1] == 'E' && header_[2] == 'S' && header_[3] == 0x1A;
}

bool ROM::hasTrainerArea() {
    return header_[6] & 0x04;
}

u16 ROM::getPRGSize() {
    return header_[4] * 1024 * 16;
}

u16 ROM::getCHRSize() {
    return header_[5] * 1024 * 8;
}

u8 ROM::getFlag6() {
    return header_[6];
}

u8 ROM::getFlag7() {
    return header_[7];
}
