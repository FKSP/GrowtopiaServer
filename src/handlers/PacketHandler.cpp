#include "PacketHandler.h"

using namespace std;

void handle_packets(ENetPeer* peer, ENetPacket* packet, player* player) {
    int type = get_packet_type(packet);
    gamepacket* p;
    string data;
    std::stringstream ss;
    std::string to;
    bool is_joinreq = false;

    switch (type) {
        default:
            printf("Unknown type %d\n", type);
            break;
        case 2:
            data = get_text(packet);

            if (!player->is_loggedin())
                return handle_login(player, data);

            data = get_text(packet);
            if (data == "action|refresh_item_data") {
                // Ignored; idk how to use it. i'm just rewriting a full server. Maybe you could help me ^^
            }
            break;
        case 3:
            data = get_text(packet);
            cout << data << endl;
            ss = stringstream(data);
            while (std::getline(ss, to, '\n')) {
                string id = to.substr(0, to.find('|'));
                string act = to.substr(to.find('|') + 1, to.length() - to.find('|') - 1);

            }
            break;
        case 4:
            break;
    }
    enet_packet_destroy(packet);
}

int get_packet_type(ENetPacket *packet) {
    int result = 0;

    if (packet->dataLength > 3u)
        result = *(packet->data);
    else
        printf("Bad packet length, ignoring packet\n");

    return result;
}

char* get_text(ENetPacket* packet)
{
    char zero = 0;
    memcpy(packet->data + packet->dataLength - 1, &zero, 1);
    return (char*)(packet->data + 4);
}
