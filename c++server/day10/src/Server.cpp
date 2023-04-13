#include "Server.h"
#include "Socket.h"
// #include "InetAddress.h"
//#include "Epoll.h"
//#include "util.h"
// #include "Channel.h"
#include "Acceptor.h"
#include "Connection.h"
#include <functional>
// #include "string.h"
// #include <unistd.h>



// #define READ_BUFFER 1024    



Server::Server(EventLoop* _loop) : loop(_loop), acceptor(nullptr)
{
    // Socket *serv_sock = new Socket();
    // InetAddress *serv_addr = new InetAddress("127.0.0.1",8888);
    // serv_sock->bind(serv_addr);
    // serv_sock->listen();
    // serv_sock->setnonblocking();
    //--7
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    // Channel *serverChannel = new Channel(loop,serv_sock->getFd());
    // std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);     
    // //在类中，使用bind()绑定类的成员函数时，需要引入this作为第一个参数，用来访问当前所在的类。其他参数和绑定静态参数时相同。
    // //bind()函数的第一个参数为需要绑定的函数，后面为该函数需要传入的参数。
    // //参数的传入，可以直接传入值，也可以使用占位符
    // serverChannel->setCallback(cb);
    // serverChannel->enableReading();

}

Server::~Server()
{
    delete acceptor;
}
//  connection --echo----替代
// void Server::handleReadEvent(int sockfd)
// {
//     char buf[READ_BUFFER];
//     while(true)
//     {
//         //由于使用非阻塞IO，读取客户端buffer，一次读取buf大小数据，直到全部读取完毕
//         bzero(&buf,sizeof(buf));
//         ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
//         if (bytes_read > 0)
//         {
//             printf("message from client fd %d: %s\n", sockfd, buf);
//             write(sockfd, buf, sizeof(buf));
//         }
//         else if (bytes_read == -1 && errno == EINTR) 
//         {
//             //客户端正常中断、继续读取
//             printf("continue reading");
//             continue;
//         }
//         else if (bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
//         {
//             //非阻塞IO，这个条件表示数据全部读取完毕
//             printf("finish reading once, errno: %d\n", errno);
//             break;
//         }
//         else if (bytes_read == 0)
//         {  //EOF，客户端断开连接
//             printf("EOF, client fd %d disconnected\n", sockfd);
//             close(sockfd);   //关闭socket会自动将文件描述符从epoll树上移除
//             break;
//         }
//     }
// }


void Server::newConnection(Socket *sock){
    // InetAddress *clnt_addr = new InetAddress();      //会发生内存泄露！没有delete
    // Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));       //会发生内存泄露！没有delete
    // printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
    // clnt_sock->setnonblocking();
    // Channel *clntChannel = new Channel(loop, clnt_sock->getFd());
    // std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
    // clntChannel->setCallback(cb);
    // clntChannel->enableReading();


    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;

}

void Server::deleteConnection(Socket *sock)
{
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}
