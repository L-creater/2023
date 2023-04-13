#ifndef CHANNEL_H
#define CHANNEL_H
#include <sys/epoll.h>

class Epoll;
class Channel       //将一个文件描述符封装成一个Channel类，一个Channel类自始至终只负责一个文件描述符，
                    //对不同的服务、不同的事件类型，都可以在类中进行不同的处理，
                    //而不是仅仅拿到一个int类型的文件描述符。
{
private:
    Epoll *ep;
    int fd;
    uint32_t events;        //希望监听这个文件描述符的那些事件
    uint32_t revents;       //表示epoll返回该channel时文件描述符正在发生的事件
    bool inEpoll;           //当前的Channek是否已经在epoll红黑树中， ---注册Channel的时候方便使用
        //  typedef union epoll_data 
        // {
        //   void *ptr;
        //   int fd;
        //   uint32_t u32;
        //   uint64_t u64;
        // } epoll_data_t;
public:
    Channel(Epoll *_ep, int _fd);   //新建一个Channel时，必须说明该Channel与哪个epoll和fd绑定：
                                    //这时该Channel还没有被添加到epoll红黑树，因为events没有被设置，
                                    //不会监听该Channel上的任何事件发生。
    ~Channel();


    void enableReading();           //如Channel不在epoll红黑树中，则添加，否则直接更新Channel、打开允许读事件。
    
    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();
    
    //void serEvents(uint32_t);
    void setRevents(uint32_t);
};

#endif