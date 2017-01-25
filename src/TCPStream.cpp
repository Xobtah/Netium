//
// Created by xobtah on 23/01/17.
//

#include "TCPStream.hpp"

namespace Netium
{
    /*
     *   Ctor & Dtor
     */
    TCPStream::TCPStream(int fd, struct sockaddr_in* address) : _sock(fd)
    {
        static_cast<void>(address);
        /*char ip[50];
		inet_ntop(AF_INET, reinterpret_cast<in_addr*>(&(address->sin_addr.s_addr)), ip, sizeof(ip) - 1);
        _ip = ip;
        _port = ntohs(address->sin_port);*/
    }

    TCPStream::~TCPStream() {}

    /*
     *  Public member functions
     */

    int TCPStream::Send(void *buffer, unsigned int len) { return (_sock.Send(buffer, len)); }
    int TCPStream::Recv(void *buffer, unsigned int len) { return (_sock.Recv(buffer, len)); }

    std::string TCPStream::GetIP() { return (_ip); }
    int         TCPStream::GetPort() { return (_port); }

    void    TCPStream::AddInFdSet(fd_set *fs) const { _sock.AddInFdSet(fs); }
    bool    TCPStream::IsInFdSet(fd_set *fs) const { return (_sock.IsInFdSet(fs)); }
    bool    TCPStream::IsSock(int fd) const { return (_sock.IsSock(fd)); }
}