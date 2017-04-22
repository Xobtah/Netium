//
// Created by xobtah on 25/01/17.
//

#include <cstring>
#include <iostream>

#include "../src/ClientData.hpp"

#include "../hdr/Netium.hpp"

int main()
{
    ium::Server  server;

    server.On("connexion", [&](ium::ClientData *client)
    {
        std::cout << client->GetId() << ": " << "Connexion" << std::endl;
        client->On("data", [=, &server](uint8_t *data)
        {
            std::cout << client->GetId() << ": " << data << std::flush;
            server.SendPacket(*client, data, PACKET_MAX_SIZE);
        });
    }).On("disconnect", [](ium::ClientData *client)
    {
        std::cout << client->GetId() << ": " << "Disconnect" << std::endl;
    });
    server.Listen().Join();
    return (0);
}