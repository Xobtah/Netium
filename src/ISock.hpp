#ifndef ISOCK_HPP_
#define ISOCK_HPP_

#include "SockInclude.hpp"

namespace Netium
{
	class ISock
	{
	public:
		virtual ~ISock() {}

		/*
		**  Init(std::string protocole)
		**  Parameter: The protocole to use ("TCP" or "UDP")
		**  Error: Throws string
		*/
		virtual void    Init(std::string protocole = "TCP") = 0;

		/*
		**  Bind(int port)
		**  Parameter: Port to bind sock
		**  Error: Throws string
		*/
		virtual void    Bind(int) = 0;

		/*
		**  Listen(int queue)
		**  Parameter: Queue size
		**  Error: Throws string
		*/
		virtual void    Listen(int) = 0;

		/*
		**  Accept(struct sockaddr_in *client)
		**  Parameter: client stuffs
		**  Returns: Client's fd
		**  Error: Throws string
		*/
		virtual int     Accept(struct sockaddr_in *client = NULL) = 0;

		/*
		**  Connect(std::string host, int port)
		**  Parameters: Host & port
		**  Error: Throws string
		*/
		virtual void    Connect(std::string, int) = 0;

		/*
		**  Send(void *buffer, unsigned int size)
		**  Parameters: Pointer to the buffer to send, the size of the buffer
		**  Returns: Number of bytes sent
		*/
		virtual int     Send(void*, unsigned int) const = 0;

		/*
		**  Recv(void *buffer, unsigned int size)
		**  Parameters: Pointer to the buffer to stock data, thee number of bytes to read
		**  Returns: Number of bytes received
		*/
		virtual int     Recv(void*, unsigned int) const = 0;

		/*
		**  SendTo(void *buffer, unsigned int size, const struct sockaddr *dest_addr)
		**  Parameters: Pointer to the buffer to send, the size of the buffer, destination address
		**  Returns: Number of bytes sent
		*/
		virtual int     SendTo(void*, unsigned int, const struct sockaddr*) const = 0;

		/*
		**  RecvFrom(void *buffer, unsigned int size, struct sockaddr *src_addr)
		**  Parameters: Pointer to the buffer to stock data, the number of bytes to read, thing
		**  Returns: Number of bytes received
		*/
		virtual int     RecvFrom(void*, unsigned int, struct sockaddr*) const = 0;

		/*
		**  AddInFdSet(fd_set *set)
		**  Parameter: Pointer to the fd set to fill
		*/
		virtual void    AddInFdSet(fd_set*) const = 0;

		/*
		**  IsInFdSet(fd_set *set)
		**  Parameter: Pointer to the fd set to test
		**  Returns: True if sock is in fd set, false if not
		*/
		virtual bool    IsInFdSet(fd_set*) const = 0;

		/*
		**  IsSock(int fd)
		**  Parameter: Fd to compare with sock
		**  Returns: True if fd is equal to the sock, false if not
		*/
		virtual bool    IsSock(int) const = 0;

		/*
		**  GetPort()
		** Returns: Port used (userful for UDP)
		*/
		virtual int     GetPort() const = 0;
	};
}

#endif // !ISOCK_HPP_