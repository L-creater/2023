#include "solider.h"

Solider::Solider(std::string name)
{
    this->name_ = name;
    this->ptr_gun_ = nullptr;
}

Solider::~Solider()
{
    if (this->ptr_gun_ == nullptr)
    {
        return;
    }
    delete this->ptr_gun_;
    this->ptr_gun_ = nullptr;
}

void Solider::addGun(Gun *ptr_gun)
{
    this->ptr_gun_ = ptr_gun;
}
void Solider::addBullerToGun(int num)
{
    this->ptr_gun_->addBullet(num);
}

bool Solider::fire()
{
    return (this->ptr_gun_->shoot());
}