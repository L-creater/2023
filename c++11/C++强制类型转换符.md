# C++中强制类型转换操作符

1. static_cast
2. dynamic_cast
3. const_cast
4. reinterpert_cast

## static_cats

- **编译器隐式执行的任何类型转换都可以由static_cast来完成，比如int与float、double与char、enum与int之间的转换等。**

```c
//static_cast 将一个表达式强制转换为指定的目标类型，前提是在编译时可以确定转换的安全性。它主要用于以下几种情况：
int num = 10;
double result = static_cast<double>(num); // 将整数转换为浮点数

int* ptr = new int(5);
long longPtr = static_cast<long>(ptr); // 将指针转换为长整型

```

- static_cast也可以用在于基类与派生类指针或引用类型之间的转换。然而它不做运行时的检查，不如dynamic_cast安全。static_cast仅仅是依靠类型转换语句中提供的信息来进行转换，而dynamic_cast则会遍历整个类继承体系进行类型检查,因此dynamic_cast在执行效率上比static_cast要差一些。

```c
class ANIMAL
{
public:
    ANIMAL():_type("ANIMAL"){};
    virtual void OutPutname(){cout<<"ANIMAL";};
private:
    string _type ;
};
class DOG:public ANIMAL
{
public:
    DOG():_name("大黄"),_type("DOG"){};
    void OutPutname(){cout<<_name;};
    void OutPuttype(){cout<<_type;};
private:
    string _name ;
    string _type ;
};

int main()
{
    //基类指针转为派生类指针,且该基类指针指向基类对象。
    ANIMAL * ani1 = new ANIMAL ;
    DOG * dog1 = static_cast<DOG*>(ani1);
    //dog1->OutPuttype();//错误，在ANIMAL类型指针不能调用方法OutPutType（）；在运行时出现错误。

    //基类指针转为派生类指针，且该基类指针指向派生类对象
    ANIMAL * ani3 = new DOG;
    DOG* dog3 = static_cast<DOG*>(ani3);
    dog3->OutPutname(); //正确

    //子类指针转为派生类指针
    DOG *dog2= new DOG;
    ANIMAL *ani2 = static_cast<DOG*>(dog2);
    ani2->OutPutname(); //正确，结果输出为大黄

    //
    system("pause");

}
```

- 使用static_cast可以找回存放在void\*指针中的值。

```c
    double a = 1.999;
    void * vptr = & a;
    double * dptr = static_cast<double*>(vptr);
    cout<<*dptr<<endl;//输出1.999
```

- ***\*static_cast\**把任何类型的表达式转换成void类型。**
- **另外，与const_cast相比，static_cast不能把换掉变量的const属性，也包括volitale或者__unaligned属性。**