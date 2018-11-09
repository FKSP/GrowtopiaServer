#include "udp_server.h"
#include "../handlers/PacketHandler.h"

#pragma clang diagnostic ignored "-Wmissing-noreturn"

using namespace std;
using namespace growtopia;

udp_server::udp_server(uint16_t port) {
    this->port = port;
}

void udp_server::handle_event(ENetEvent* event) {
    ENetPeer* peer = event->peer;

    switch(event->type) {
        case ENET_EVENT_TYPE_CONNECT:
            peer->data = new objects::player(peer);
            send_data(peer, 1, nullptr, 0);
            break;
        case ENET_EVENT_TYPE_RECEIVE:
            if (((objects::player*)peer->data)->is_updating)
            {
                cout << "packet drop" << endl;
                break;
            }

            handle_packets(this->server, peer, event->packet, (objects::player*)peer->data);
            break;
        case ENET_EVENT_TYPE_DISCONNECT:
            delete (objects::player*)peer->data;
            peer->data = nullptr;
            break;
        default: break;
    }
}

void udp_server::run(size_t slots) {
    ENetAddress address;
    enet_address_set_host(&address, "0.0.0.0");
    address.port = this->port;

    enet_initialize();

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
            /*
            try {

            } catch (const runtime_error& re) {
                enet_peer_disconnect_later(event.peer, 0);
                cerr << "Runtime error: " << re.what() << endl;
            } catch (const exception& ex) {
                enet_peer_disconnect_later(event.peer, 0);
                cerr << "Error occurred: " << ex.what() << endl;
            } catch (...) {
                enet_peer_disconnect_later(event.peer, 0);
                cout << "Unknown failure occurred. Possible memory corruption" << endl;
            }
             */
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
