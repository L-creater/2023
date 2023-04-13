#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include "ThreadPool.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr),threadPool(nullptr) ,quit(false)
{
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop()
{
   
    delete ep;
}

void EventLoop::loop()  //调用loop()函数可以开始事件驱动，实际上就是原来的程序中调用epoll_wait()函数的死循环：
{
    while (!quit)
    {
        std::vector<Channel*> chs;
        chs = ep->poll();               //poll --epoll_wait--return activeEvent
        for (auto it = chs.begin();it < chs.end() ; ++it)
        {
            (*it)->handleEvent();
        }
        
    }
    
}

void EventLoop::updateChannel(Channel *ch)
{
    ep->updateChannel(ch);
}
void EventLoop::addThread(std::function<void()> func) 
{
    threadPool->add(func);
}
