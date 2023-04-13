#include <iostream>
#include <thread>

class Singleton
{
    /**
     *Singleton 的构造函数应该始终是私有的，以防止直接
     *
     *使用 `new` 运算符进行构造调用。
     */

protected:
    Singleton(const std::string value) : value_(value) {}
    std::string value_;
    static Singleton *singleton_;

public:
    /**
     * Singletons should not be cloneable.
     */
    Singleton(Singleton &other) = delete;
    /**
     * Singletons should not be assignable.
     */
    void operator=(const Singleton &) = delete;
    /**
     *这是控制单例访问的静态方法实例。\
     *在第一次运行时，它创建一个单例对象并将其放置
     *进入静态领域。在后续运行中，它返回现有的客户端
     *对象存储在静态字段中。
     */
    static Singleton *GetInstance(const std::string &value);
    /**
     * Finally, any singleton should define some business logic, which can be
     * executed on its instance.
     */
    void SomeBusinessLogic()
    {
        //
    }

    std::string value() const
    {
        return value_;
    }
};

Singleton *Singleton::singleton_ = nullptr;

/**
 * Static methods should be defined outside the class.
 */

Singleton *Singleton::GetInstance(const std::string &value)
{
    /**
     * This is a safer way to create an instance. instance = new Singleton is
     * dangeruous in case two instance threads wants to access at the same time
     */
    if (singleton_ == nullptr)
    {
        singleton_ = new Singleton(value);
    }
    return singleton_;
}

void ThreadFoo()
{
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton *singleton = Singleton::GetInstance("FOO");
    std::cout << singleton->value() << "\n";
}

void ThreadBar()
{
    // Following code emulates slow initialization.
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    Singleton *singleton = Singleton::GetInstance("BAR");
    std::cout << singleton->value() << "\n";
}

int main()
{
    std::cout << "If you see the same value, then singleton was reused (yay!\n"
              << "If you see different values, then 2 singletons were created (booo!!)\n\n"
              << "RESULT:\n";
    std::thread t1(ThreadFoo);
    std::thread t2(ThreadBar);
    t1.join();
    t2.join();

    return 0;
}