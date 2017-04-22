//
// Created by xobtah on 08/02/17.
//

#ifndef NETIUM_CLIENTDATA_HPP
#define NETIUM_CLIENTDATA_HPP

#include <string>
#include <vector>

#include "TCPStream.hpp"

#include "../Jasonium/src/Json.hpp"
#include "../Poolium/src/Mutex.hpp"
#include "../Emitium/src/EventEmitter.hpp"

#define PACKET_MAX_SIZE	512

namespace ium
{
    class   ClientData : public Jasonium::Json, public ium::EventEmitter
    {
    public:
        ClientData();
        ClientData(unsigned int, TCPStream*);
        ClientData(const ClientData&);
        ClientData  &operator=(const ClientData&);
        ~ClientData();

        TCPStream   *GetStream() const;
        unsigned int    &GetId();

    private:
        unsigned int    _id;
        TCPStream		*_stream;
    };
}

#endif //NETIUM_CLIENTDATA_HPP
