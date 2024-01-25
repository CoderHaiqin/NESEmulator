#ifndef IOREGISTER_H
#define IOREGISTER_H
#include "type.h"
#include <vector>

class IORegister
{
private:
    u8 keyState1;
    u8 keyState2;
    bool* keyState_;

    bool strobe_;
public:
    IORegister();
    void bindKeyState(bool* keyState);
    void update();

    void write(u16 addr, u8 value);
    u8 read(u16 addr);
};

#endif // IOREGISTER_H
