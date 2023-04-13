#include "Base.cpp"
#include "Component.cpp"
class Drived : public Base
{
private:
  
public:
    Component *c1;
    Drived()
    {   
         std::cout << "Drive 构造函数"<<std::endl;
    }
    virtual ~Drived(){
        delete c1;
        std::cout << "Drive 析构函数"<<std::endl;
}

};




