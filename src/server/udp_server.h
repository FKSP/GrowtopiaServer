#ifndef GROWTOPIASERVER_UDP_SERVER_H
#define GROWTOPIASERVER_UDP_SERVER_H

#include <ostream>
#include <enet/enet.h>

#include "../objects/player.h"

class udp_server {
public:
    explicit udp_server(uint16_t port);

    void run(size_t slots);
private:
    uint16_t port;
    ENetHost* server;

    void handle_event(ENetEvent* event);
    void send_data(ENetPeer* peer, int num, char* data, size_t len);
};


#endif //GROWTOPIASERVER_UDP_SERVER_H
