#pragma once
#include <iostream>

class Gun{
public:
    Gun(std::string type)
    {
        this->bullet_count_ = 0;
        this->type_ = type;
    }

    void addBullet(int buller_num);
    bool shoot();
private:
    int bullet_count_;
    std::string type_;
};