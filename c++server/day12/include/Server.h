#pragma once
#include <vector>
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server
{
private:
    // EventLoop *loop;        //事件循环

    EventLoop *mainReactor;   //只负责接受连接，然后分发给一个subReactor
    Acceptor *acceptor;     //用于接收TCP连接
    std::map<int, Connection*> connections;  //所有TCP连接 
                                            // 键为该连接客户端的socket fd，值为指向该连接的指针。
    std::vector<EventLoop*> subReactors;
    ThreadPool *thpool;     //线程池
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(int sockfd);
};
