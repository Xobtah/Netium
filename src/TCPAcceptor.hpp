//
// Created by xobtah on 23/01/17.
//

#ifndef ZIA_TCPACCEPTOR_HPP
#define ZIA_TCPACCEPTOR_HPP

#include <string>

#include "TCPStream.hpp"

namespace ium
{
    class TCPAcceptor
    {
      public:
        TCPAcceptor();
        TCPAcceptor(int, int);
        ~TCPAcceptor();

        TCPAcceptor &InitSock(int, int);
        TCPStream   *Accept();

        void    AddInFdSet(fd_set*) const;
        bool    IsInFdSet(fd_set*) const;
        bool    IsSock(int) const;

      private:
        OSSOCK      _sock;
    };
}

#endif //ZIA_TCPACCEPTOR_HPP
