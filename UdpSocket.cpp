#include "UdpSocket.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>

#include <iostream>

UdpSocket::UdpSocket() {

    ready_ = false;
    sock_ = -1;

    // Default values
    remoteAddress_ = "127.0.0.1";
    remotePort_ = 5010;
    localPort_ = 5001;
}

UdpSocket::~UdpSocket() {

}

void UdpSocket::LocalPort(const int value) {
    localPort_ = value;
}

void UdpSocket::RemoteAddress(const std::string & value) {

    remoteAddress_ = value;
}

void UdpSocket::RemotePort(const int value) {

    remotePort_ = value;
}

bool UdpSocket::Init() {

    sock_ = socket(AF_INET, SOCK_DGRAM | SOCK_NONBLOCK, IPPROTO_UDP);
    if (sock_ != -1) {
        // Non-blocking I/O
        int noBlock;
        int flags = fcntl(sock_, F_GETFL, 0);
        fcntl(sock_, F_SETFL, flags | O_NONBLOCK);

        memset((char *)&remoteHost_, 0, sizeof(remoteHost_));
        remoteHost_.sin_family = AF_INET;
        remoteHost_.sin_port = htons(remotePort_);
        if (inet_aton(remoteAddress_.c_str(), &remoteHost_.sin_addr) != 0) {

            int optval = 1;
            if (setsockopt(sock_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval)) == 0) {
                localHost_.sin_family = AF_INET;
                localHost_.sin_port = htons(localPort_);
                localHost_.sin_addr.s_addr = htonl(INADDR_ANY);

                if (bind(sock_, (sockaddr*)&localHost_, sizeof(localHost_)) != -1)
                {
                    ready_ = true;
                }
                else
                {
                    std::cerr << "Unable to bind socket!" << std::endl;
                    throw;
                }
            }
            else
            {
                perror("setsockopt");
            }
        }
        else
        {
            std::cerr << "Unable to resolve address!" << std::endl;
            throw;
        }
    }
    else
    {
        std::cerr << "Unable to create socket!" << std::endl;
        throw;
    }

    return ready_;
}

int UdpSocket::Write(const uint8_t* buffer, int size) {

    int bytesSent = -1;
    int slen = sizeof(remoteHost_);
    bytesSent = sendto(sock_, buffer, size, 0, (const sockaddr *) &remoteHost_, slen);

    if (bytesSent == -1) {
        std::cerr << "Unable to send data!" << std::endl;
    }

    return bytesSent;
}

int UdpSocket::Read(uint8_t* buffer, int size)
{
    int rxSize = -1;
    sockaddr_in remoteAddr;
    socklen_t slen = sizeof(remoteAddr);

    rxSize = recvfrom(sock_, buffer, size, 0, (sockaddr*)&remoteAddr, &slen);
    if (rxSize == -1) {
        if (errno != EAGAIN) {
            std::cerr << "Unable to receive data!" << std::endl;
        }
    }

    return rxSize;
}

