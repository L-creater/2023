#include "include/EventLoop.h"
#include "include/Server.h"

int main()
{
    EventLoop *loop = new EventLoop();      //new epoll
    Server *server = new Server(loop);      //new ---Socket---Inetaddress---bind---listen---setnonblocking---Channel
                                            //---回调函数---accept---clnt_sock-setnonblock---bind(回调函数，handleReadEvent)
    loop->loop();                           //epoll_wait
                                        
    /*
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    clntChannel->setCallback(cb);
    clntChannel--cb--handleReadEvent
    */
    /*
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    serverChannel->setCallback(cb);
    //serverChannel--cb--newConnection --accept
    */

    return 0;
}