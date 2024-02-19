#include "ioregister.h"

IORegister::IORegister() {

    strobe_ = 0;
}

u8 IORegister::read(u16 addr) {
    u8 result = 0;
    if(addr == 0x16) {
        if (strobe_){
            result = keyState_[0];
        }else{
            //
            result = keyState1 & 1;
            keyState1 >>= 1;
        }

        return 0x40 | result;
    }
    return 0;
}

void IORegister::write(u16 addr, u8 value) {
    if(addr == 0x16) {
        bool strobe_old = strobe_;
        strobe_ = value & 1;
        if (strobe_old && !(strobe_)){
            update();
        }
    }
}

void IORegister::update() {
    keyState1 = 0;
    for (int i = 0; i <= 7; i++){
        if (keyState_[i]) {
            keyState1 |= (1 << i);
        }
    }
}

void IORegister::bindKeyState(bool* keyState) {
    keyState_ = keyState;
}
