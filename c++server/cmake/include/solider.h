#pragma once

#include <string>
#include "gun.h"
class Solider{
public:
    Solider(std::string name);
    ~Solider();
    void addGun(Gun* );
    void addBullerToGun(int num);
    bool fire(); 
private:
    std::string name_;
    Gun* ptr_gun_;
};