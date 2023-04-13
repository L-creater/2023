#include <iostream>
using namespace std;

//程序库开发人员
class Library
{
private:
    
public:
    //稳定 template method
    //run --稳定的   step 2 4变化
   void Run(){
        Step1();

        if (Step2()) 
        {
            //支持变化 ==》虚函数的多态调用
            Step3();
        }

        for (int i = 0; i < 4; i++)
        {
            Step4();    //支持变化 ==》虚函数的多态调用
        }
        
        Step5();
   }

    virtual ~Library(){}

   protected:
      void Step1()
    {
        //稳定
    }
    void Step3()
    {
        //稳定
    }
    void Step5()
    {
        //稳定
    }
    virtual bool  Step2()=0;
  
    virtual void  Step4()=0;
    
};

