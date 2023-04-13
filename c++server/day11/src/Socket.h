#ifndef SOCKET_H
#define SOCKET_H

class InetAddress;
class Socket
{
private:
    int fd;
public:
    Socket();
    Socket(int);
    ~Socket();
 
    void bind(InetAddress*);
    void listen();
    void connect(InetAddress*);
    void setnonblocking();
    
    int accept(InetAddress*);
    int getFd();

};



#endif