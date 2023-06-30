# std::bind和std::function解析

`std::function` 是 C++ 中的一个模板类，用于封装可调用对象（如函数、函数指针、函数对象、Lambda 表达式等），并提供统一的调用接口。它可以用于保存和传递不同类型的可调用对象，并允许在运行时动态地改变所存储对象的类型。

1. 包含头文件：

   ```
   #include <functional>
   ```

2. 创建 `std::function` 对象：

   ```
   std::function<返回类型(参数类型)> func;
   ```

   其中，`返回类型` 是可调用对象的返回类型，`参数类型` 是可调用对象的参数类型。例如，如果要保存一个返回整数、接受两个整数参数的函数，可以这样声明：

   ```c
   std::function<int(int, int)> func;
   ```



1. 普通函数
2. lambda表达式(匿名函数)
3. 函数指针
4. 仿函数(functor 重载括号运算符实现)
5. 类成员函数
6. 静态成员函数

```c++
#include <iostream>
#include <functional>

using namespace std;

std::function<bool(int, int)> fun;

//普通函数
bool compare_com(int a, int b)
{
    return a > b;
}


//lambda表达式
auto compare_lambda = [](int a, int b){ return a > b;};


//仿函数
class compare_class
{
public:
    bool operator()(int a, int b)
    {
        return a > b;
    }
};

//类成员函数
class compare
{
public:
    bool compare_member(int a, int b)
    {
        return a > b;
    }
    static bool compare_static_member(int a, int b)
    {
        return a > b;
    }
};


int main()
{
    bool result;
    fun = compare_com;
    result = fun(10, 1);
    cout << "普通函数输出, result is " << result << endl;

    fun = compare_lambda;
    result = fun(10, 1);
    cout << "lambda表达式输出, result is " << result << endl;

    fun = compare_class();
    result = fun(10, 1);
    cout << "仿函数输出, result is " << result << endl;

    fun = compare::compare_static_member;
    result = fun(10, 1);
    cout << "类静态成员函数输出, result is " << result << endl;

    // 类普通成员函数比较特殊，需要使用bind函数，并且需要实例化对象，成员函数要加取地址符
    compare temp;
    fun = std::bind(&compare::compare_member, temp, std::placeholders::_1, std::placeholders::_2);
    result = fun(10, 1);
    cout << "类普通成员函数输出, result is " << result << endl;
    
    // 类成员函数也可以采用这种办法
    std::function<bool(compare*, int, int)> func = &compare::compare_member;
    compare compare_;
    func(&compare_, 10, 1);
    cout << "类普通成员函数输出, result is " << result << endl;
   }
   

  
               

```



```c++
#include <functional>
#include <iostream>
 
using namespace std;
struct Int 
{
    int a;
};
 
bool compare_com(struct Int a, float b)
{
    return a.a > b;
}
 
int main()
{
    Int a = {1};
    //placeholders::_1对应float, placeholders::_2对应struct Int所以返回值fun的类型为function<bool(float, Int)>
    std::function<bool(float, struct Int)> fun = bind(compare_com, placeholders::_2, placeholders::_1);
    bool result = fun(2.0, a);
    cout << "result is " << result << endl;
    return 0;
}
```

