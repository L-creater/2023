//新接口
class ITarget{
public:
    virtual void process() = 0;
   
};
//老接口
class IAdaptee{
public:
    virtual void foo(int data) = 0;
    virtual int bar() = 0;
};
//遗留类型
class OldClass : public IAdaptee{
    OldClass(){}
    ~OldClass(){}
    //....
};

//对象适配器
class Adapter : public ITarget{
    IAdaptee* pAdaptee;
public:
    Adapter(IAdaptee *pAdaptee){
        this->pAdaptee = pAdaptee;
    }
     
    virtual void process(){
        int data = pAdaptee->bar();
        pAdaptee->foo(data);
    }
};

int main(){
    IAdaptee *pAdaptee = new OldClass();

    ITarget* pTarget = new Adapter(pAdaptee);
    pTarget->process();
}