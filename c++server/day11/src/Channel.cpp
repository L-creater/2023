#include "Channel.h"

#include <unistd.h>

#include "EventLoop.h"

// Channel::Channel(Epoll *_ep, int _fd) : ep(_ep),fd(_fd), events(0), revents(0), inEpoll(false)
// {
// }

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true)
{
}

Channel::~Channel()
{
    if( fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent()
{
    // loop->addThread(writeCallback);
    // //callback();
    if(ready & (EPOLLIN | EPOLLPRI)){
        if(useThreadPool)
            loop->addThread(readCallback);
        else
            readCallback();
    }
    if(ready & (EPOLLOUT)){
        if(useThreadPool)
            loop->addThread(writeCallback);
        else    
            writeCallback();
    }

}
void Channel::enableRead() //如Channel不在epoll红黑树中，则添加，否则直接更新Channel、打开允许读事件。
{
    events |= EPOLLIN | EPOLLET;
    // ep->updateChannel(this);
    loop->updateChannel(this);
}

int Channel::getFd()
{
    return fd;
}

uint32_t Channel::getEvents()
{
    return events;
}

uint32_t Channel::getready()
{
    return ready;
}

bool Channel::getInEpoll()  //确认是否在红黑树上
{
    return inEpoll;
}

void Channel::setInEpoll(bool _in) // 设置在红黑树上
{
    inEpoll = _in;
}

void Channel::useET()
{
    events |= EPOLLET;
    loop->updateChannel(this);
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setReady(uint32_t _ev)
{
    ready = _ev;
}


void Channel::setReadCallback(std::function<void()> _cb)
{
    readCallback = _cb;
}

void Channel::setUseThreadPool(bool use)
{
    useThreadPool = use;
}
