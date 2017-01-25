//
// Created by xobtah on 11/12/16.
//

#ifndef STRUCT_HPP_
#define STRUCT_HPP_

#include "TCPStream.hpp"

namespace Netium
{
	struct ClientStruct
	{
		ClientStruct() : _id(0), stream(NULL) {}
		ClientStruct(unsigned int _id, TCPStream *stream) : _id(_id), stream(stream) {}

		unsigned int    _id;
		TCPStream		*stream;
	};
}

#endif // !STRUCT_HPP_