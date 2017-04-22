//
// Created by xobtah on 23/01/17.
//

#ifndef ZIA_TCPSTREAM_HPP
#define ZIA_TCPSTREAM_HPP

#include <string>

#include "SockInclude.hpp"

namespace ium
{
    class TCPStream
    {
    public:
        friend class TCPAcceptor;

        ~TCPStream();

        int Send(const uint8_t*, unsigned int);
        int Recv(uint8_t*, unsigned int);

        std::string GetIP();
        int         GetPort();

        void    AddInFdSet(fd_set*) const;
        bool    IsInFdSet(fd_set*) const;
        bool    IsSock(int) const;

    private:
        TCPStream(int, sockaddr_in*);
        OSSOCK          _sock;
        std::string     _ip;
        int             _port;
    };
}

#endif //ZIA_TCPSTREAM_HPP
