#include "gun.h"

void Gun::addBullet(int bull_num)
{
    this->bullet_count_ += bull_num;
}

bool Gun::shoot()
{
    if (this->bullet_count_ <= 0)
    {
        std::cout << "no bullet" << std::endl;
        return false;
    }
    this->bullet_count_ -= 1;
    std::cout << "shoot" << std::endl;
    return true;
}