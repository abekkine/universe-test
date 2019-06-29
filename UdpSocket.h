#ifndef UTIL_UDP_SOCKET_H_
#define UTIL_UDP_SOCKET_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <string>

class UdpSocket {
private:
    bool ready_;
    std::string remoteAddress_;
    int remotePort_;
    int localPort_;
    sockaddr_in remoteHost_;
    sockaddr_in localHost_;
    int sock_;

public:
    UdpSocket();
    ~UdpSocket();
    void RemoteAddress(const std::string & value);
    void RemotePort(const int value);
    void LocalPort(const int value);
    int Read(uint8_t* buffer, int size);
    int Write(const uint8_t* buffer, int size);
    bool Init();
};
    
#endif // UTIL_UDP_SOCKET_H_

