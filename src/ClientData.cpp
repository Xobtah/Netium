//
// Created by xobtah on 08/02/17.
//

#include "ClientData.hpp"

namespace ium
{
    /*
     *  Ctor & Dtor
     */

    ClientData::ClientData() : _id(0), _stream(NULL) {}

    ClientData::ClientData(unsigned int _id, TCPStream *stream) : _id(_id), _stream(stream) {}

    ClientData::ClientData(const ClientData &cd) : ium::EventEmitter(cd), _id(cd._id), _stream(cd._stream) {}

    ClientData  &ClientData::operator=(const ClientData &cd)
    {
        if (&cd == this)
            return (*this);
        _id = cd._id;
        _stream = cd._stream;
        return (*this);
    }

    ClientData::~ClientData() {}

    /*
     *  Public member functions
     */

    TCPStream   *ClientData::GetStream() const { return (_stream); }

    unsigned int    &ClientData::GetId() { return (_id); }
}