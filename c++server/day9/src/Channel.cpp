#include "Channel.h"
#include "EventLoop.h"
#include <unistd.h>
// Channel::Channel(Epoll *_ep, int _fd) : ep(_ep),fd(_fd), events(0), revents(0), inEpoll(false)
// {
// }

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false)
{
}

Channel::~Channel()
{
    if(fd != -1)
        close(fd);
        fd = -1;
}

void Channel::handleEvent()
{
    callback();
}
void Channel::enableReading() //如Channel不在epoll红黑树中，则添加，否则直接更新Channel、打开允许读事件。
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

uint32_t Channel::getRevents()
{
    return revents;
}

bool Channel::getInEpoll()  //确认是否在红黑树上
{
    return inEpoll;
}

void Channel::setInEpoll() // 设置在红黑树上
{
    inEpoll = true;
}

// void Channel::setEvents(uint32_t _ev){
//     events = _ev;
// }

void Channel::setRevents(uint32_t _ev)
{
    revents = _ev;
}


void Channel::setCallback(std::function<void()> _cb)
{
    callback = _cb;
}
