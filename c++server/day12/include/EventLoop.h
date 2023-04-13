#ifndef EVENTLOOP_H
#define EVENTLOOP_H
#include <functional>

class Epoll;
class Channel;
class ThreadPool;
class EventLoop             //main-Reactor
{
private:
    Epoll *ep;
    ThreadPool *threadPool;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();    //调用loop()函数可以开始事件驱动，实际上就是原来的程序中调用epoll_wait()函数的死循环：
    void updateChannel(Channel*);
    
    void addThread(std::function<void()> );
};

#endif