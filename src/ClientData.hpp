//
// Created by xobtah on 08/02/17.
//

#ifndef NETIUM_CLIENTDATA_HPP
#define NETIUM_CLIENTDATA_HPP

#include <string>
#include <vector>
#include <queue>

#include "TCPStream.hpp"

#include "../Jasonium/src/Json.hpp"
#include "../Poolium/src/Mutex.hpp"
#include "../Emitium/src/EventEmitter.hpp"

#define PACKET_MAX_SIZE	512

namespace Netium
{
    class   ClientData : public Jasonium::Json, public Emitium::EventEmitter
    {
    public:
        ClientData();
        ClientData(unsigned int, TCPStream*);
        ClientData(const ClientData&);
        ClientData  &operator=(const ClientData&);
        ~ClientData();

        TCPStream   *GetStream() const;
        unsigned int    &GetId();

        ClientData  &Push(std::string);
        const std::string   &Front() const;
        ClientData  &Pop();

    private:
        unsigned int    _id;
        TCPStream		*_stream;
        Poolium::Mutex  _mutex;
        std::queue<std::string> _buffers;
    };
}

#endif //NETIUM_CLIENTDATA_HPP
