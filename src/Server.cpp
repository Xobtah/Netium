//
// Created by xobtah on 23/01/17.
//

#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>

#include "Server.hpp"
#include "ProtocolException.hpp"

namespace ium
{
    /*
     *  Ctor & Dtor
     */

    Server::Server()
    try : _clients([](ClientData &c) { delete c.GetStream(); }), _timeOut(500000) {}
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    Server::Server(Basium::DataBase<ClientData> &db)
    try : _clients(db, [](ClientData &c) { delete c.GetStream(); }), _timeOut(500000) {}
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    Server::~Server() {}

    /*
     *  Public member functions
     */

    Server  &Server::Listen(int port, int queue)
    {
        _acceptor.InitSock(port, queue);
        _thread.Set([&]() { this->ThreadRunner(); }).Run();
        return (*this);
    }

    Server  &Server::SendPacket(ClientData const &client, const uint8_t *packet, unsigned int size)
    {
        if (!client.GetStream())
            throw ServerException("Client stream is null");
        client.GetStream()->Send(packet, size);
        return (*this);
    }

    unsigned int    Server::GetTimeOut() const { return (_timeOut); }

    void            Server::SetTimeOut(unsigned int to) { _timeOut = to; }

    Server  &Server::Join()
    {
        _thread.Join();
        return (*this);
    }

    /*
     *  Private membre functions
     */

    int Server::CreateFdSet(fd_set *fs)
    {
        std::vector<ClientData*>    subdb = _clients.Find([](const ClientData &c)
                                                            {
                                                                static_cast<void>(c);
                                                                return (true);
                                                            });

        FD_ZERO(fs);
        _acceptor.AddInFdSet(fs);
        for (std::vector<ClientData*>::iterator it = subdb.begin(); it != subdb.end(); it++)
            (*it)->GetStream()->AddInFdSet(fs);
        return (static_cast<int>(subdb.size() + 4));
    }

    int Server::OSSelect(int maxfd, fd_set *readfd, fd_set *writefd, struct timeval *to, fd_set *exceptfd)
    { return (select(maxfd, readfd, writefd, exceptfd, to)); }

    void    Server::NewClient() try
    {
        unsigned int    id = 0;
        ClientData c(id, _acceptor.Accept());

        id = _clients.Insert(c);
        _clients.FindOne(id).GetId() = id;
        this->Emit("connexion", &_clients.FindOne(id));
    }
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }

    void    Server::RemoveClient(unsigned int _id) try
    {
        this->Emit("disconnect", &_clients.FindOne(_id));
        _clients.Remove(_id);
    }
    catch (Basium::DataBaseException const &dbe) { static_cast<void>(dbe); }

    void    Server::ThreadRunner() try
    {
        while (42)
        {
            fd_set      rfs;
            int         max = this->CreateFdSet(&rfs);
            timeval     timeout;
            ClientData  *bufCli = NULL;
            uint8_t     packet[PACKET_MAX_SIZE];

            timeout.tv_sec = 0;
            timeout.tv_usec = _timeOut;
            if (this->OSSelect(max, &rfs, NULL, &timeout) <= 0)
                continue;
            if (_acceptor.IsInFdSet(&rfs))
            {
                this->NewClient();
                continue;
            }
            memset(packet, 0, PACKET_MAX_SIZE);
            bufCli = &_clients.FindOne([](const ClientData &c, void *rfs)
                                       {
                                           return (c.GetStream()->IsInFdSet(reinterpret_cast<fd_set*>(rfs)));
                                       }, &rfs);
            if (!bufCli->GetStream()->Recv(packet, PACKET_MAX_SIZE) || !static_cast<uint16_t>(*packet))
            {
                this->RemoveClient(bufCli->GetId());
                continue;
            }
            bufCli->Emit("data", packet);
        }
    }
    catch (ProtocolException const &pe) { throw ServerException("ProtocolException: " + std::string(pe.what())); }
    catch (Basium::DataBaseException const &dbe) { std::cerr << "DataBaseException: " + std::string(dbe.what()) << std::endl; }
}