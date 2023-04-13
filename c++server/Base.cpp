#include <iostream>

class Base
{
private:
    int i=0;
public:
    Base();
    virtual ~Base(){
        std::cout << "Base 析构函数"<<std::endl;
    }
};

Base::Base()
{
    std::cout << "Base 构造函数"<<std::endl;
}

