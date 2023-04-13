#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>
#include <functional>

class Socket;
class EventLoop;
class Channel
{
private:
    // Epoll *ep;
    EventLoop *loop;
    int fd;
    uint32_t events;
    uint32_t ready;
    bool inEpoll;
    //---6
    // std::function<void()> callback;
    //11
    bool useThreadPool;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;

public:
    //Channel(Epoll *_ep, int _fd);
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    //---6
    void handleEvent();
    void enableRead();

    int getFd();
    uint32_t getEvents();
    uint32_t getready();
    bool getInEpoll();
    void setInEpoll(bool _in = true);
    //11
    void useET();
    
    //void setEvents(uint32_t);
    void setReady(uint32_t);
    //---6
    // void setCallback(std::function<void()>);

    //11
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};





#endif