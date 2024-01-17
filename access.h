#ifndef ACCESS_H
#define ACCESS_H
#include "type.h"

class Access
{
public:
    Access();

    virtual u8 read() = 0;
    virtual void write(u8 value) = 0;
};

class BusAccess : public Access {
public:
    u8 read() override;
    void write(u8 value) override;
};

class RegistersAccess : public Access {
public:

    u8 read() override;
    void write(u8 value) override;
};

#endif // ACCESS_H
