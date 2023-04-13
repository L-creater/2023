#ifndef INETADDRESS_H
#define INETADDRESS_H

#include <arpa/inet.h>

class InetAddress
{
private:
    
public:
    struct sockaddr_in addr;
    socklen_t addr_len;
    InetAddress();
    InetAddress(const char* ip, uint16_t port);
    ~InetAddress();
};



#endif