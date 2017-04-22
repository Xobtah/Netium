//
// Created by xobtah on 23/01/17.
//

#include "TCPAcceptor.hpp"
#include "ProtocolException.hpp"
#include "SockException.hpp"
#include "TCPStream.hpp"

namespace ium
{
    /*
     *  Ctor & Dtor
     */

    TCPAcceptor::TCPAcceptor() try : _sock() {}
    catch (SockException const &se) { throw ProtocolException("SockException: " + std::string(se.what())); }

    TCPAcceptor::TCPAcceptor(int port, int queue) try : _sock()
    {
        _sock.Init();
        _sock.Bind(port);
        _sock.Listen(queue);
    }
    catch (SockException const &se) { throw ProtocolException("SockException: " + std::string(se.what())); }

    TCPAcceptor::~TCPAcceptor() {}

    /*
     *  Public member functions
     */

    TCPAcceptor &TCPAcceptor::InitSock(int port, int queue)
    try
    {
        _sock.Init();
        _sock.Bind(port);
        _sock.Listen(queue);
        return (*this);
    }
    catch (SockException const &se) { throw ProtocolException("SockException: " + std::string(se.what())); }

    TCPStream   *TCPAcceptor::Accept() try
    {
        sockaddr_in addr;
        int fd = 0;

        fd = _sock.Accept(&addr);
        return (new TCPStream(fd, &addr));
    }
    catch (SockException const &se) { throw ProtocolException("SockException: " + std::string(se.what())); }

    void    TCPAcceptor::AddInFdSet(fd_set *fs) const { _sock.AddInFdSet(fs); }
    bool    TCPAcceptor::IsInFdSet(fd_set *fs) const { return (_sock.IsInFdSet(fs)); }
    bool    TCPAcceptor::IsSock(int fd) const { return (_sock.IsSock(fd)); }
}