#ifndef WINSOCK_HPP_
#define WINSOCK_HPP_

#include "SockInclude.hpp"
#include "ISock.hpp"

namespace ium
{
	class   WinSock : public ISock
	{
	public:
		WinSock();
		WinSock(int);
		~WinSock();

		void    Init(std::string protocol = "TCP");
		void    Bind(int);
		void    Listen(int);
		int     Accept(struct sockaddr_in *client = NULL);
		void    Connect(std::string, int);

		int     Send(void*, unsigned int) const;
		int     Recv(void*, unsigned int) const;
		int     SendTo(void*, unsigned int, const struct sockaddr*) const;
		int     RecvFrom(void*, unsigned int, struct sockaddr*) const;

		void    AddInFdSet(fd_set*) const;
		bool    IsInFdSet(fd_set*) const;
		bool    IsSock(int) const;
		virtual int     GetPort() const;

	private:
		SOCKET  _socket;
		int     _port;
		addrinfo *_info;
		std::string _protocol;
		WSADATA _wsaData;
	};
}

#endif // !WINSOCK_HPP_