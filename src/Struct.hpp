//
// Created by xobtah on 11/12/16.
//

#ifndef STRUCT_HPP_
#define STRUCT_HPP_

#include <vector>
#include <array>

#include "TCPStream.hpp"

#define PACKET_MAX_SIZE	512

namespace Netium
{
	struct ClientStruct
	{
		ClientStruct() : _id(0), stream(NULL) {}
		ClientStruct(unsigned int _id, TCPStream *stream) : _id(_id), stream(stream) {}

		unsigned int    _id;
		TCPStream		*stream;
		std::vector<std::array<uint8_t, PACKET_MAX_SIZE> >	buf;
	};
}

#endif // !STRUCT_HPP_