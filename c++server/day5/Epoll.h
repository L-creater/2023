#ifndef EPOLL_h
#define EPOLL_H
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll
{
private:
    int epfd;
    struct epoll_event *events;
public:

    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    // std::vector<epoll_event> poll(int timeout = -1);
   /*
    注意在成员列表中初始化的顺序并不是列表顺序 而是：
    在类中声明的顺序！
    在成员初始化列表中有前后顺序依赖的时候一定要注意！
   */
     void updateChannel(Channel*);
    std::vector<Channel*> poll(int timeout = -1);
 
};



#endif