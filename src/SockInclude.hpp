#ifndef SOCKINCLUDE_HPP_
#define SOCKINCLUDE_HPP_

#ifdef __linux__

/*
**  struct protoent
**  getprotobyname
*/
#include <netdb.h>
/*
**  htons
*/
#include <arpa/inet.h>
/*
**  socket
**  bind
**  listen
**  accept
**  connect
*/
#include <sys/socket.h>
/*
**  sys/socket+
**  select
**  fd_set
*/
#include <sys/types.h>
/*
**  select
**  fd_set
*/
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include "UnixSock.hpp"

#define OSSOCK Netium::UnixSock

#endif



#ifdef _WIN32

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WIN32_WINNT  0x501
#include <winsock2.h>
#include <Ws2tcpip.h>
#include "WinSock.hpp"

#define OSSOCK Netium::WinSock

#endif

#endif