#include "Channel.h"
#include "Epoll.h"

Channel::Channel(Epoll *_ep, int _fd) : ep(_ep),fd(_fd), events(0), revents(0), inEpoll(false)
{
}

Channel::~Channel()
{
}

void Channel::enableReading()       //如Channel不在epoll红黑树中，则添加，否则直接更新Channel、打开允许读事件。
{
    events = EPOLLIN | EPOLLET;     //将要监听的事件events设置为读事件并采用ET模式
    ep->updateChannel(this);        //ep指针指向的Epoll红黑树中更新该Channel
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