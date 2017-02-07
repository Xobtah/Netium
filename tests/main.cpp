//
// Created by xobtah on 25/01/17.
//

#include <iostream>

#include "../hdr/Netium.hpp"

int main()
{
    Netium::Server  server;

    server.On("connexion", [](void *client) -> void*
    {
        (void) client;
        std::cout << "New client" << std::endl;
        return (NULL);
    }).On("disconnect", [](void *client) -> void*
    {
        (void) client;
        std::cout << "Rem client" << std::endl;
        return (NULL);
    });
    server().Join();
    return (0);
}