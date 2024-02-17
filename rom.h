#ifndef ROM_H
#define ROM_H
#include "type.h"
#include <string>
#include <vector>

class ROM {
public:
    std::vector<u8> header_;
    std::vector<u8> trainer_;
    std::vector<u8> PRG_;
    std::vector<u8> CHR_;
    std::vector<u8> M_;

    u8 mapperType_;

    ROM();
    bool load(const std::string& path);

    bool check();
    bool hasTrainerArea();
    u16 getPRGSize();
    u16 getCHRSize();
    u8 getFlag6();
    u8 getFlag7();
};

#endif // ROM_H
