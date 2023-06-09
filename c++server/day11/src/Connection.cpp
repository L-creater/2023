#include "Connection.h"

#include <string.h>
#include <iostream> 
#include <unistd.h>

#include "Socket.h"
#include "Channel.h"
#include "util.h"
#include "Buffer.h"


#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr)
{
    channel = new Channel(loop,sock->getFd());
    channel->enableRead();
    channel->useET();
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd()); 
    channel->setReadCallback(cb);
    channel->setUseThreadPool(true);
    readBuffer = new Buffer();
}

Connection::~Connection()
{
    delete channel;
    delete readBuffer;
    delete sock;
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb)
{
    deleteConnectionCallback = _cb;
}

void Connection::echo(int sockfd)
{
    // 回显sockfd发来的数据
    char buf[1024]; //这个buf大小无所谓
    while (true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            readBuffer->append(buf, bytes_read);
        }
        else if (bytes_read == -1 && errno== EINTR)
        {
            std::cout<<"continue reading"<<std::endl;
            continue;
        }
        else if (bytes_read == -1 &&((errno == EAGAIN) || (errno == EWOULDBLOCK))) 
        {
            //非阻塞IO，这个条件表示数据全部读取完毕
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            // errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "socket write error");
            send(sockfd);
            readBuffer->clear();
            break;
        }
        else if(bytes_read == 0)
        {
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sockfd);           //多线程会有bug
            break;
        }
        else 
        {
            printf("connection reset by peer\n");
            deleteConnectionCallback(sockfd);  //会有bug，注释后单线程无bug
            break;        
        }
        
        
    }
    
}
void Connection::send(int sockfd)
{
    char buf[readBuffer->size()];
    strcpy(buf,readBuffer->c_str());
    int data_size = readBuffer->size();
    int data_left = data_size;
    while (data_left > 0)
    {   
        ssize_t bytes_write = write(sockfd, buf + data_size-data_left, data_left);
        if(bytes_write == -1 && (errno == EAGAIN))
        {
            break;
        }
        data_left -= bytes_write;
    }
    
}

