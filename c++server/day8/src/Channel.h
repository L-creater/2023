#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel
{
private:
    // Epoll *ep;
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t revents;
    bool inEpoll;
    //---6
    std::function<void()> callback;
public:
    //Channel(Epoll *_ep, int _fd);
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    //---6
    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();
    
    //void serEvents(uint32_t);
    void setRevents(uint32_t);
    //---6
    void setCallback(std::function<void()>);
};





#endif