# C++并发编程

## 第一章 你好，c++的并发世界

### 入门

```c++
//清单 1.1 一个简单的Hello, Concurrent World程序：

#include <iostream>
#include <thread>  //①
void hello()  //②
{
  std::cout << "Hello Concurrent World\n";
}
int main()
{
  std::thread t(hello);  //③
  t.join();  //④
}
```

第一个区别是增加了`#include <thread>`①,标准C++库中对多线程支持的声明在新的头文件中：管理线程的函数和类在`<thread>`中声明，而保护共享数据的函数和类在其他头文件中声明。

其次，打印信息的代码被移动到了一个独立的函数中②。因为每个线程都必须具有一个*初始函数*(initial function)，新线程的执行从这里开始。对于应用程序来说，初始线程是main()，但是对于其他线程，可以在`std::thread`对象的构造函数中指定——本例中，被命名为t③的`std::thread`对象拥有新函数hello()作为其初始函数。

下一个区别：与直接写入标准输出或是从main()调用hello()不同，该程序启动了一个全新的线程来实现，将线程数量一分为二——初始线程始于main()，而新线程始于hello()。

新的线程启动之后③，初始线程继续执行。如果它不等待新线程结束，它就将自顾自地继续运行到main()的结束，从而结束程序——有可能发生在新线程运行之前。这就是为什么在④这里调用`join()`的原因——详见第2章，这会导致调用线程(在main()中)等待与`std::thread`对象相关联的线程，即这个例子中的t。

## 线程管理

### 2.1 线程管理基础

每个程序至少有一个线程：执行main()函数的线程，其余线程有其各自的入口函数。线程与原始线程(以main()为入口函数的线程)同时运行。如同main()函数执行完会退出一样，当线程执行完入口函数后，线程也会退出。在为一个线程创建了一个`std::thread`对象后，需要等待这个线程结束；不过，线程需要先进行启动。下面就来启动线程。

**第1章中，线程在`std::thread`对象创建(为线程指定任务)时启动。**

```C++
void do_some_work();
std::thread my_thread(do_some_work);
```

`std::thread`可以用可调用类型构造，将带有函数调用符类型的实例传入`std::thread`类中，替换默认的构造函数。

```c++
class background_task
{
public:
  void operator()() const
  {
    do_something();
    do_something_else();
  }
};

background_task f;
std::thread my_thread(f);
```

**有件事需要注意，**当把函数对象传入到线程构造函数中时，需要避免“[最令人头痛的语法解析](http://en.wikipedia.org/wiki/Most_vexing_parse)”(*C++’s most vexing parse*, [中文简介](http://qiezhuifeng.diandian.com/post/2012-08-27/40038339477))。如果你传递了一个临时变量，而不是一个命名的变量；C++编译器会将其解析为函数声明，而不是类型对象的定义。

例如：

```
std::thread my_thread(background_task());
```

这里相当与声明了一个名为my_thread的函数，这个函数带有一个参数(函数指针指向没有参数并返回background_task对象的函数)，返回一个`std::thread`对象的函数，而非启动了一个线程。

使用在前面命名函数对象的方式，或使用多组括号①，或使用新统一的初始化语法②，可以避免这个问题。

如下所示：

```
std::thread my_thread((background_task()));  // 1
std::thread my_thread{background_task()};    // 2
```

**启动了线程，你需要明确是要等待线程结束(*加入式*——参见2.1.2节)，还是让其自主运行(*分离式*——参见2.1.3节)。**

### 2.1.2 等待线程完成

`t.join()`

### 2.1.3 特殊情况下的等待

需要对一个还未销毁的`std::thread`对象使用join()或detach()。如果想要分离一个线程，可以在线程启动后，直接使用detach()进行分离。如果打算等待对应线程，则需要细心挑选调用join()的位置。当在线程运行之后产生异常，在join()调用之前抛出，就意味着这次调用会被跳过。

通常，当倾向于在无异常的情况下使用join()时，需要在异常处理过程中调用join()，从而避免生命周期的问题。

## 2.2 向线程函数传递参数

向`std::thread`构造函数中的可调用对象，或函数传递一个参数很简单。需要注意的是，默认参数要拷贝到线程独立内存中，即使参数是引用的形式，也可以在新线程中进行访问。再来看一个例子：

```c
void f(int i, std::string const& s);
std::thread t(f, 3, "hello");
```

代码创建了一个调用f(3, "hello")的线程。注意，函数f需要一个`std::string`对象作为第二个参数，但这里使用的是字符串的字面值，也就是`char const *`类型。之后，在线程的上下文中完成字面值向`std::string`对象的转化。需要特别要注意，当指向动态变量的指针作为参数传递给线程的情况，