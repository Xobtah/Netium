#include "UnixSock.hpp"
#include "SockException.hpp"

namespace ium
{
    /*
    **  Ctor & Dtor
    */

    UnixSock::UnixSock() : _socket(0), _port(0) {}

    UnixSock::UnixSock(int fd) : _socket(fd), _port(0) {}

    UnixSock::~UnixSock() { close(_socket); }

    /*
    **  Member functions
    */

    void    UnixSock::Init(std::string protocol)
    {
        struct protoent       *prot;
        this->_protocol = protocol;
        int lol = 1;

        // Getting TCP prototype
        if (protocol != "TCP" && protocol != "UDP")
            throw SockException("Protocol must be TCP or UDP");
        if (!(prot = getprotobyname(protocol.c_str())))
            throw SockException("Failed to getprotobyname");
        // Getting socket
        if (!_socket)
        {
            if ((_socket = socket(AF_INET, protocol == "TCP" ? SOCK_STREAM : SOCK_DGRAM, prot->p_proto)) == -1)
                throw SockException("Failed to init socket");
        }
        setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &lol, sizeof(int));
    }

    void    UnixSock::Bind(int port)
    {
        struct sockaddr_in    sock;

        // Init port
        _port = port;
        if (_port <= 1024 || _port > 65535)
            throw SockException("Port must be a value between 1024 and 65535");
        // Init stuff
        sock.sin_family = AF_INET;
        sock.sin_port = htons(static_cast<uint16_t>(_port));
        sock.sin_addr.s_addr = INADDR_ANY;
        // Binding socket
        if (bind(_socket, reinterpret_cast<const struct sockaddr*>(&sock), sizeof(sock)) == -1)
            throw SockException("Failed to bind socket on port " + std::to_string(_port));
        if (this->_protocol == "UDP")
        {
            struct sockaddr_in tmp;
            socklen_t oui = sizeof(tmp);
            if (getsockname(this->_socket, reinterpret_cast<sockaddr *>(&tmp), &oui) == -1)
                throw SockException("Failed on getsockname");
            this->_port = tmp.sin_port;
        }
    }

    void    UnixSock::Listen(int queue)
    {
        // Init queue
        if (listen(_socket, queue) == -1)
            throw SockException("Failed to listen");
    }

    int UnixSock::Accept(sockaddr_in *client)
    {
        if (!client)
            throw SockException("Client is null");
        unsigned int    clientLength = sizeof(*client);
        int fd;

        // Accepting new client
        if ((fd = accept(_socket, reinterpret_cast<struct sockaddr*>(client), &clientLength)) == -1)
            throw SockException("Accept failed");
        return (fd);
    }

    void    UnixSock::Connect(std::string host, int port)
    {
        struct sockaddr_in  sin;
        struct hostent  *hostinfo = NULL;

        if ((hostinfo = gethostbyname(host.c_str())) == NULL)
            throw SockException("Unknown host");
        sin.sin_addr = *(in_addr*) hostinfo->h_addr;
        sin.sin_port = htons(static_cast<uint16_t >(port));
        sin.sin_family = AF_INET;
        if (connect(_socket, (sockaddr*) &sin, sizeof(sockaddr)) == -1)
            throw SockException("Connect failed");
    }

    int UnixSock::Send(const void *buffer, unsigned int size) const { return (static_cast<int>(send(_socket, buffer, size, 0))); }
    int UnixSock::Recv(void *buffer, unsigned int size) const { return (static_cast<int>(recv(_socket, buffer, size, 0))); }
    int UnixSock::SendTo(void *buffer, unsigned int size, const struct sockaddr *addr) const
    { return (static_cast<int>(sendto(_socket, buffer, size, 0, addr, sizeof(*addr)))); }
    int UnixSock::RecvFrom(void *buffer, unsigned int size, struct sockaddr *addr) const
    {
        socklen_t sinsize = sizeof(*addr);

        return (static_cast<int>(recvfrom(_socket, buffer, size, 0, addr, &sinsize)));
    }

    void    UnixSock::AddInFdSet(fd_set *set) const { FD_SET(_socket, set); }
    bool    UnixSock::IsInFdSet(fd_set *set) const { return (FD_ISSET(_socket, set)); }
    bool    UnixSock::IsSock(int fd) const { return (_socket == fd); }

    int UnixSock::GetPort() const { return (this->_port); }
}
