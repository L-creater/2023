#include <iostream>

class A{
public:
    A(){
        std::cout<<"A 构造函数"<< std::endl;
    }
    virtual ~A(){
         std::cout<<"A 析构函数"<< std::endl;
    }
    void fun_A(){
        std::cout<<"A 类的成员函数" <<std::endl;
    }
    virtual void accomplish(){};

    std::string get_str(){
        std::cout<<str_<<std::endl;
        return str_;
    }
private:
    std::string str_{"lyt"};
};

class B : public A{
public:
    B(){
        std::cout<<"B 构造函数"<< std::endl;
    }
    ~B(){
         std::cout<<"B 析构函数"<< std::endl;
    }

    void fun_B(){
        std::cout<<"B 的成员函数"<<std::endl;
    }
    virtual void accomplish(){
        std::cout<<"调用B的虚函数"<< std::endl;
        // return get_str();
    }
private:
    int i=0;
};

int main()
{
    A *a = new B();
 
    A *a2 = new A();
    
    a2->get_str();
    a->get_str();

    a->fun_A();
 
    a->accomplish();
    // a->fun_B();   //无法调用
    return 0;
}