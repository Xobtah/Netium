#ifndef UNIXSOCK_HPP_
#define UNIXSOCK_HPP_

#include <string>
#include "SockInclude.hpp"
#include "ISock.hpp"

namespace Netium
{
	class UnixSock : public ISock
	{
	public:
		UnixSock();
		UnixSock(int);
		~UnixSock();

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
		std::string _protocol;
		int _socket;
		int _port;
	};
}

#endif // !UNIXSOCK_HPP_