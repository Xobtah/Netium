//
// Created by xobtah on 23/01/17.
//

#ifndef ZIA_SERVER_HPP
#define ZIA_SERVER_HPP

#include <map>

#include "SockInclude.hpp"
#include "TCPStream.hpp"
#include "TCPAcceptor.hpp"
#include "ClientData.hpp"
#include "../Basium/DataBase.hpp"

#include "../Poolium/src/Thread.hpp"

#include "../Emitium/src/EventEmitter.hpp"

#define PACKET_MAX_SIZE 512

namespace ium
{
    // ServerException
    class ServerException : public std::exception
    {
    public:
        ServerException(std::string what) throw() : _what(what) {}
        ~ServerException() {}

        virtual const char  *what() const throw() { return _what.c_str(); }

    private:
        std::string _what;
    };

    // Server
    class Server : public ium::EventEmitter
    {
    public:
        Server();
        Server(Basium::DataBase<ClientData>&);
        ~Server();

        Server  &Listen(int port = 4242, int queue = 42);

        Server  &SendPacket(ClientData const&, const uint8_t*, unsigned int);

        unsigned int    GetTimeOut() const;
        void            SetTimeOut(unsigned int);

        Server  &Join();

    private:
        Basium::DataBase<ClientData>  _clients;
        TCPAcceptor         _acceptor;
        Poolium::Thread     _thread;
        unsigned int        _timeOut;

        int     CreateFdSet(fd_set*);
        int     OSSelect(int, fd_set*, fd_set *writefd = NULL, struct timeval *to = NULL, fd_set *exceptfd = NULL);
        void    NewClient();
        void    RemoveClient(unsigned int);
        void    ThreadRunner();
    };
}

#endif //ZIA_SERVER_HPP
