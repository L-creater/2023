#pragma once

#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server
{
private:
    EventLoop *loop;        //事件循环
    Acceptor *acceptor;     //用于接收TCP连接
    std::map<int, Connection*> connections;  //所有TCP连接 
                                            // 键为该连接客户端的socket fd，值为指向该连接的指针。
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket* sock);
};
