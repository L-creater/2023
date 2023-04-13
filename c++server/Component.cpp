#include <iostream>

class Component
{
private:
    int j=1;
public:
    Component();
    ~Component();
};

Component::Component()
{
    std::cout <<"Component 构造函数" <<std::endl;
}

Component::~Component()
{   
     std::cout <<"Component 析构函数" <<std::endl;
}
