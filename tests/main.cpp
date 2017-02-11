//
// Created by xobtah on 25/01/17.
//

#include <iostream>
#include <utility>

#include "../hdr/Netium.hpp"

int main()
{
    Netium::Server  server;

    server.On("connexion", [](Netium::ClientData *c)
    {
        std::cout << c->GetId() << ": " << "Connexion" << std::endl;
        c->On("data", [=]()
        {
            std::cout << c->GetId() << ": " << c->Front();
            c->Pop();
        });
    }).On("disconnect", [](Netium::ClientData *c)
    {
        std::cout << c->GetId() << ": " << "Disconnect" << std::endl;
    });
    server().Join();
    return (0);
}