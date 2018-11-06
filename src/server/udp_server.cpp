#include "udp_server.h"
#include "../handlers/PacketHandler.h"

#pragma clang diagnostic ignored "-Wmissing-noreturn"

using namespace std;

udp_server::udp_server(uint16_t port) {
    enet_initialize();

    this->port = port;
}

void udp_server::handle_event(ENetEvent* event) {
    ENetPeer* peer = event->peer;

    switch(event->type) {
        case ENET_EVENT_TYPE_CONNECT:
            peer->data = new player(peer);
            send_data(peer, 1, nullptr, 0);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            handle_packets(peer, event->packet, (player*)peer->data);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            delete (player*)peer->data;
            peer->data = nullptr;
            break;
        default: break;
    }
}

void udp_server::run(size_t slots) {
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = this->port;

    this->server = enet_host_create(&address, slots, 10, 0, 0);

    if (this->server == nullptr)
    {
        fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
        exit(1);
    }

    this->server->checksum = enet_crc32;
    enet_host_compress_with_range_coder(this->server);

    ENetEvent event;

    printf("Server should be listening at port %d\n", this->port);
    while (true)
        while (enet_host_service(server, &event, 1000) > 0)
            this->handle_event(&event);
}

void udp_server::send_data(ENetPeer* peer, int num, char* data, size_t len)
{
    ENetPacket * packet = enet_packet_create(nullptr, len + 5, ENET_PACKET_FLAG_RELIABLE);
    memcpy(packet->data, &num, 4);
    if (data != nullptr)
        memcpy(packet->data+4, data, len);
    char zero = 0;
    memcpy(packet->data + 4 + len, &zero, 1);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(this->server);
}
