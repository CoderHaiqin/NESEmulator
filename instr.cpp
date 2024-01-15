#include "instr.h"
#include "constant.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <qstring.h>

Instr::Instr(Registers* registers, Bus* bus) : registers_(registers), bus_(bus)
{
    this->func_ = [](Registers* registers, Bus* bus) {
        std::cout << "illegal instrction" << std::endl;
    };
}

Instr::Instr(Registers* registers, Bus* bus, std::function<void(Registers*, Bus*)> func)
    : registers_(registers), bus_(bus), func_(func)
{

}

bool Instr::execute() {
    this->func_(this->registers_, this->bus_);
}

void InstrGenerator::init(const std::string& filePath, Registers* registers, Bus* bus) {
    instrTypeTable.resize(Constant::instrTableSize);
    cycleNumTable.resize(Constant::instrTableSize);
    table.resize(Constant::instrTableSize);
    for(int i = 0; i < Constant::instrTableSize; i++) {
        cycleNumTable[i].resize(Constant::instrTableSize);
        instrTypeTable[i].resize(Constant::instrTableSize);
        table[i].resize(Constant::instrTableSize);
    }

    std::ifstream fs;
    fs.open(filePath);
    if(!fs.is_open()) {
        std::cout << "fail to open " << filePath << std::endl;
        return;
    }

    std::string line;
    int i = 0;
    while(std::getline(fs, line)) {
        if(line[0] == '#') {
            continue;
        }
        std::stringstream ss(line);
        for(int j = 0; j < Constant::instrTableSize; j++) {
            int tmp = 0;
            ss >> tmp;
            cycleNumTable[i][j] = tmp;
        }
        i++;
        if(i == Constant::instrTableSize) {
            break;
        }
    }
    for(int i = 0; i < Constant::instrTableSize; i++) {
        for(int j = 0; j < Constant::instrTableSize; i++) {
            
        }
    }


    table[0][0] = new Instr(registers, bus, [](Registers* registers, Bus* bus) {
        u8 opCode = bus->read(registers->PC);

    });
}
