//
// Created by xobtah on 23/01/17.
//

#include <cstring>
#include <string>
#include <iostream>

#include "Server.hpp"
#include "ProtocolException.hpp"

namespace Netium
{
    /*
     *  Ctor & Dtor
     */

    Server::Server(int port, int queue)
    try : _clients([](ClientStruct &c) { delete c.stream; }), _acceptor(port, queue), _thread(*this), _timeOut(500000) {}
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    Server::Server(Basium::DataBase<ClientStruct> &db, int port, int queue)
    try : _clients(db, [](ClientStruct &c) { delete c.stream; }), _acceptor(port, queue), _thread(*this), _timeOut(500000) {}
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    Server::~Server() {}

    /*
     *  Public member functions
     */

    int     Server::SendPacket(ClientStruct const &client, uint8_t *packet, unsigned int size) { return (client.stream->Send(packet, size)); }

    unsigned int    Server::GetTimeOut() const { return (_timeOut); }

    void            Server::SetTimeOut(unsigned int to) { _timeOut = to; }

    Poolium::Thread &Server::operator()() { return (_thread); }

    /*
     *  Private membre functions
     */

    int Server::CreateFdSet(fd_set *fs)
    {
        std::vector<ClientStruct*>    subdb = _clients.Find([](const ClientStruct &c)
                                                            {
                                                                static_cast<void>(c);
                                                                return (true);
                                                            });

        FD_ZERO(fs);
        _acceptor.AddInFdSet(fs);
        for (std::vector<ClientStruct*>::iterator it = subdb.begin(); it != subdb.end(); it++)
            (*it)->stream->AddInFdSet(fs);
        return (static_cast<int>(subdb.size() + 4));
    }

    int Server::OSSelect(int maxfd, fd_set *readfd, fd_set *writefd, struct timeval *to, fd_set *exceptfd)
    { return (select(maxfd, readfd, writefd, exceptfd, to)); }

    bool    Server::NewClient() try
    {
        unsigned int    id = 0;
        ClientStruct c(id, _acceptor.Accept());

        std::cout << "New client" << std::endl;
        id = _clients.Insert(c);
        _clients.FindOne(id)._id = id;
        return (false);
    }
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    bool    Server::RemoveClient(unsigned int _id) try
    {
        _clients.Remove(_id);
        std::cout << "Rem client" << std::endl;
        return (false);
    }
    catch (Basium::DataBaseException const &dbe) { static_cast<void>(dbe); return (false); }

    void    Server::ThreadRunner() try
    {
        while (42)
        {
            fd_set  rfs;
            int max = this->CreateFdSet(&rfs);
            timeval timeout;
            ClientStruct    *bufCli = NULL;
            uint8_t         packet[PACKET_MAX_SIZE];

            timeout.tv_sec = 0;
            timeout.tv_usec = _timeOut;
            switch (this->OSSelect(max, &rfs, NULL, &timeout))
            {
                case -1:
                    continue;
                case 0:
                    continue;
                default:
                    break;
            }
            if (_acceptor.IsInFdSet(&rfs))
            {
                this->NewClient();
                continue;
            }
            memset(packet, 0, PACKET_MAX_SIZE);
            bufCli = &_clients.FindOne([](const ClientStruct &c, void *rfs)
                                       {
                                           return (c.stream->IsInFdSet(reinterpret_cast<fd_set*>(rfs)));
                                       }, &rfs);
            if (!bufCli->stream->Recv(packet, PACKET_MAX_SIZE) || !static_cast<uint16_t>(*packet))
            {
                this->RemoveClient(bufCli->_id);
                continue;
            }
            std::cout << packet << std::endl;
        }
    }
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }
    catch (Basium::DataBaseException const &dbe) { std::cerr << "Select: " + std::string(dbe.what()) << std::endl; }
}