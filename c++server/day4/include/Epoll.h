#ifndef EPOLL_h
#define EPOLL_H
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:

    Epoll();        //epoll_creat
    ~Epoll();

    void addFd(int fd, uint32_t op);                 //epoll_ctl
    std::vector<epoll_event> poll(int timeout = -1); //epoll_wait
};



#endif