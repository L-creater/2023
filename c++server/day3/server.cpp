#include<iostream>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include "utli.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void setnonblocking(int fd) //设置非阻塞
{
    fcntl(fd,F_SETFL,fcntl(fd,F_GETFL) | O_NONBLOCK);
}

int main()
{
   int sockfd = socket(AF_INET,SOCK_STREAM,0);
   errif(sockfd == -1, "socket creat error");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(bind(sockfd,(sockaddr*)&serv_addr,sizeof(serv_addr) )== -1,"socket bind error");

    errif(listen(sockfd,SOMAXCONN) == -1,"socket listen error");

    int epfd = epoll_create1(0);  //创建树根
    errif(epfd == -1,"epoll creat error");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(&events,sizeof(events));

    bzero(&ev,sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;  // EPOLLIN--读
    setnonblocking(sockfd); //设置非阻塞
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);   //监听文件描述符上树

   while(true)  //等待事件发生
    {
        int nfds = epoll_wait(epfd,events,MAX_EVENTS, -1); //
        errif(nfds = -1,"epoll wait error");
        for (int i = 0; i < nfds; i++)
        {
            if(events[i].data.fd == sockfd) //新客户端连接
            {
                struct sockaddr_in clnt_addr;
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr,sizeof(clnt_addr));

                int clnt_sockfd = accept(sockfd,(sockaddr*)&clnt_addr,&clnt_addr_len);
                errif(clnt_sockfd == -1,"socket accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
                
                bzero(&ev,sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET; 
                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sockfd,&ev);
            }   
            else if (events[i].events & EPOLLIN)    //可读事件
            {
               char buf[READ_BUFFER];
               while (true)
               {
                    bzero(&buf,sizeof(buf));
                    ssize_t read_bytes = read(events[i].data.fd,buf,sizeof(buf));
                    if(read_bytes > 0)
                    {
                        printf("message from client fd %dd: %s\n", events[i].data.fd,buf);
                        write(events[i].data.fd,buf,sizeof(buf));
                    }
                    else if (read_bytes == -1 && errno == EINTR)
                    {
                        printf("continue reading");
                        continue;
                    }
                    else if (read_bytes == -1 && (errno == EAGAIN) || (errno == EWOULDBLOCK))    //非阻塞io，这个条件表示数据全部读取完毕
                    {
                        printf("finish reading once, errno: %d\n",errno);
                        break;
                    }
                    else if (read_bytes == 0)
                    {
                        printf("EOF, client fd %d disconnected\n", events[i].data.fd);
                        close(events[i].data.fd);   //关闭socket会自动将文件描述符从epoll树上移除
                        break;
                    }
                }
            }
            else 
            {
                printf("something else happened\n");
            }  
        }
       
    }
}