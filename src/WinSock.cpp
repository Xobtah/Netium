#include <sstream>
#include <iostream>
#include "WinSock.hpp"
#include "SockException.hpp"

namespace ium
{
    WinSock::WinSock() : _socket(INVALID_SOCKET), _port(0) {}

    WinSock::WinSock(int fd) : _socket(fd), _port(0) {}

    WinSock::~WinSock()
    {
        if (shutdown(_socket, SD_SEND) == SOCKET_ERROR)
        {
            closesocket(_socket);
            WSACleanup();
			std::cerr << "Shutdown failed" << std::endl;
        }
        closesocket(_socket);
        WSACleanup();
    }

    void	WinSock::Init(std::string protocol)
    {
        this->_protocol = protocol;
    }

    void	WinSock::Bind(int port)
    {
        this->_port = port;
        if (WSAStartup(MAKEWORD(2, 2), &_wsaData))
            throw SockException("If you read that, Windows sucks");
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = (this->_protocol == "TCP") ? SOCK_STREAM : SOCK_DGRAM;
        hints.ai_protocol = (this->_protocol == "TCP") ? IPPROTO_TCP : IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        int result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &this->_info);
        if (result != 0)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
            WSACleanup();
            throw SockException("Failed to get addrinfo");
        }
        this->_socket = INVALID_SOCKET;
        if ((_socket = socket(this->_info->ai_family, this->_info->ai_socktype, this->_info->ai_protocol)) == INVALID_SOCKET)
        {
            freeaddrinfo(this->_info);
            WSACleanup();
            throw SockException("Failed to init socket");
        }
        if (bind(_socket, this->_info->ai_addr, static_cast<int>(this->_info->ai_addrlen)) == SOCKET_ERROR)
        {
            freeaddrinfo(this->_info);
            closesocket(_socket);
            WSACleanup();
            throw SockException("Failed to bind socket on port " + std::to_string(_port));
        }
        if (this->_protocol == "UDP")
        {
            struct sockaddr_in tmp;
            int oui = sizeof(tmp);
            if (getsockname(this->_socket, reinterpret_cast<sockaddr *>(&tmp), &oui) == SOCKET_ERROR)
            {
                closesocket(_socket);
                WSACleanup();
                throw SockException("Failed getsockname");
            }
            this->_port = tmp.sin_port;
        }
    }

    void	WinSock::Listen(int queue)
    {
        if (listen(_socket, queue) == SOCKET_ERROR)
        {
            closesocket(this->_socket);
            WSACleanup();
            throw SockException("Failed to listen");
        }
    }

    int WinSock::Accept(struct sockaddr_in *client)
    {
        if (!client)
            throw SockException("Client is null");
        int	clilen = sizeof(*client);
        SOCKET	fd;

        if ((fd = accept(_socket, nullptr, nullptr)) == INVALID_SOCKET)
        {
            closesocket(this->_socket);
            WSACleanup();
            throw SockException("Accept failed");
        }
        return (static_cast<int>(fd));
    }

    void	WinSock::Connect(std::string host, int port)
    {
        if (WSAStartup(MAKEWORD(2, 2), &_wsaData))
            throw SockException("If you read that, Windows sucks");
        struct addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = (this->_protocol == "TCP") ? SOCK_STREAM : SOCK_DGRAM;
        hints.ai_protocol = (this->_protocol == "TCP") ? IPPROTO_TCP : IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the local address and port to be used by the server
        int result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &this->_info);
        if (result != 0)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
            WSACleanup();
            throw SockException("Failed to get addrinfo");
        }
        this->_socket = INVALID_SOCKET;
        if ((_socket = socket(this->_info->ai_family, this->_info->ai_socktype, this->_info->ai_protocol)) == INVALID_SOCKET)
        {
            freeaddrinfo(this->_info);
            WSACleanup();
            throw SockException("Failed to init socket");
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the local address and port to be used by the server
        result = getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &this->_info);
        if (result != 0)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
            WSACleanup();
            throw SockException("Failed to get addrinfo");
        }
        if (connect(this->_socket, this->_info->ai_addr, static_cast<int>(this->_info->ai_addrlen)) == SOCKET_ERROR)
        {
            freeaddrinfo(this->_info);
            closesocket(_socket);
            WSACleanup();
            throw SockException("Failed to connect socket on port " + std::to_string(port));
        }
        //sockaddr_in sin;

        //sin.sin_family = AF_INET;
        //sin.sin_addr.s_addr = inet_addr(host.c_str());
        //sin.sin_port = htons(port);
        //if (connect(_socket, reinterpret_cast<sockaddr*>(&sin), sizeof(sin)) == SOCKET_ERROR)
        //	throw SockException("Connect failed");
    }

    int WinSock::Send(void *buffer, unsigned int size) const
    {
        int result = static_cast<int>(send(_socket, reinterpret_cast<char*>(buffer), size, 0));
        if (result == -1)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
        }
        return (result);
    }

    int WinSock::Recv(void *buffer, unsigned int size) const
    {
        int result = static_cast<int>(recv(_socket, reinterpret_cast<char*>(buffer), size, 0));
        if (result == -1)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
        }
        return (result);
    }
    int WinSock::SendTo(void *buffer, unsigned int size, const struct sockaddr *addr) const
    {
        int result = static_cast<int>(sendto(_socket, reinterpret_cast<char*>(buffer), size, 0, this->_info->ai_addr, static_cast<int>(this->_info->ai_addrlen)));
        if (result == -1)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
        }
        return (result);
    }
    int WinSock::RecvFrom(void *buffer, unsigned int size, struct sockaddr *addr) const
    {
        socklen_t sinsize = sizeof(*addr);
        int lol = static_cast<int>(this->_info->ai_addrlen);
        int result = static_cast<int>(recvfrom(_socket, reinterpret_cast<char*>(buffer), size, 0, this->_info->ai_addr, &lol));
        if (result == -1)
        {
            wchar_t *s = NULL;
            FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                           NULL, WSAGetLastError(),
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPWSTR)&s, 0, NULL);
            fprintf(stderr, "%S\n", s);
            LocalFree(s);
        }
        return (result);
    }

    void    WinSock::AddInFdSet(fd_set *set) const { FD_SET(_socket, set); }
    bool    WinSock::IsInFdSet(fd_set *set) const
    {
        if (FD_ISSET(_socket, set))
            return (true);
        return (false);
    }
    bool    WinSock::IsSock(int fd) const { return (_socket == static_cast<unsigned int>(fd)); }

    int WinSock::GetPort() const
    { return (this->_port); }
}
