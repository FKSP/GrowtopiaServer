#include "PacketHandler.h"

using namespace std;
using namespace growtopia;

unsigned long long client_id = 0;

void handle_packets(ENetHost* server, ENetPeer* peer, ENetPacket* packet, objects::player* player) {
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

            try {
                if (!player->is_loggedin())
                    handlers::handle_login(player, data);
            } catch (int x) {
                if (x == 1)
                    return;
            }

            data = get_text(packet);
            if (data == "action|refresh_item_data\n")
                player->send_items_db();

            data = get_text(packet);
            if (data.substr(0, 17) == "action|enter_game" && player->is_loggedin()) {
                cout << data << endl;
                player->send_world_list();
            }

            p = gamepacket::new_packet("OnDialogRequest")
                    ->write("set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|\n\nadd_spacer|small|\nadd_label_with_icon|small|`4WARNING:`` `5Worlds (and accounts)`` might be deleted at any time if database issues appear (once per day or week).|left|4|\nadd_label_with_icon|small|`4WARNING:`` `5Accounts`` are in beta, bugs may appear and they will be probably deleted often, because of new account updates, which will cause database incompatibility.|left|4|\nadd_spacer|small|\n\nadd_url_button||``Watch: `1Watch a video about GT Private Server``|NOFLAGS|https://www.youtube.com/watch?v=_3avlDDYBBY|Open link?|0|0|\nadd_url_button||``Channel: `1Watch Growtopia Noobs' channel``|NOFLAGS|https://www.youtube.com/channel/UCLXtuoBlrXFDRtFU8vPy35g|Open link?|0|0|\nadd_url_button||``Items: `1Item database by Nenkai``|NOFLAGS|https://raw.githubusercontent.com/Nenkai/GrowtopiaItemDatabase/master/GrowtopiaItemDatabase/CoreData.txt|Open link?|0|0|\nadd_url_button||``Discord: `1GT Private Server Discord``|NOFLAGS|https://discord.gg/8WUTs4v|Open the link?|0|0|\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");

            player->send_packet(p);
            break;
        case 3:
            data = get_text(packet);
            ss = stringstream(data);
            while (std::getline(ss, to, '\n')) {
                string key = to.substr(0, to.find('|'));
                string val = to.substr(to.find('|') + 1, to.length() - to.find('|') - 1);

                if (key == "action") {
                    if (val == "join_request") is_joinreq = true;
                }
                if (key == "name" && is_joinreq) {
                    database::world::world w = database::world::get_world(val);
                    if (!w.joinable) continue;
                    if (w.get_joined_peers(server) > 100) {
                        player->send_console_message("`1This world is to `4full`1! There are `^"+std::to_string(w.get_joined_peers(server))+" `1online!");
                        continue;
                    }
                    player->send_world(&w);

                    int x = 3040;
                    int y = 736;

                    for (int j = 0; j < w.width*w.height; j++)
                    {
                        if (w.blocks[j].foreground == 6) {
                            x = (j%w.width) * 32;
                            y = (j / w.width) * 32;
                        }
                    }

                    p = gamepacket::new_packet("OnSpawn")
                            ->write("spawn|avatar\n"
                                    "netID|" + std::to_string(client_id) + "\n"
                                    "userID|" + std::to_string(client_id) + "\n"
                                    "colrect|0|0|20|30\n"
                                    "posXY|" + std::to_string(x) + "|" + std::to_string(y) + "\n"
                                    "name|``" + player->display_name + "``\n"
                                    "country|" + player->country + "\n"
                                    "invis|0\n"
                                    "mstate|0\n"
                                    "smstate|0\n"
                                    "type|local\n");
                    player->send_packet(p);

                    //growtopia::objects::on_peer_connect(server, peer);

                    player->client_id = static_cast<int>(client_id);
                }
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

/*
bool is_here(ENetPeer* peer, ENetPeer* peer2)
{
    return ((player*)(peer->data))->joined_world == ((player*)(peer2->data))->joined_world;
}

void on_peer_connect(ENetHost* server, ENetPeer* peer) {
    ENetPeer * currentPeer;

    for (currentPeer = server->peers;
         currentPeer < &server->peers[server->peerCount];
         ++currentPeer)
    {
        if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
            continue;
        if (peer != currentPeer)
        {
            if (is_here(peer, currentPeer))
            {
                string netIdS = std::to_string(((player*)currentPeer->data)->client_id);
                gamepacket* p = gamepacket::new_packet("OnSpawn")
                        ->write("spawn|avatar\n"
                                "netID|" + netIdS + "\n"
                                "userID|" + netIdS + "\n"
                                "colrect|0|0|20|30\n"
                                "posXY|" + std::to_string(((player*)currentPeer->data)->posx) + "|" + std::to_string(((player*)currentPeer->data)->posy) + "\n"
                                "name|``" + ((player*)currentPeer->data)->display_name + "``\n"
                                "country|" + ((player*)currentPeer->data)->country + "\n"
                                "invis|0\n"
                                "mstate|0\n"
                                "smstate|0\n");

                ((player*)peer->data)->send_packet(p);

                string netIdS2 = std::to_string(((player*)(peer->data))->client_id);
                p = gamepacket::new_packet("OnSpawn")
                        ->write("spawn|avatar\n"
                                "netID|" + netIdS2 + "\n"
                                "userID|" + netIdS2 + "\n"
                                "colrect|0|0|20|30\n"
                                "posXY|" + std::to_string(((player*)peer->data)->posx) + "|" + std::to_string(((player*)peer->data)->posy) + "\n"
                                "name|``" + ((player*)peer->data)->display_name + "``\n"
                                "country|" + ((player*)peer->data)->country + "\n"
                                "invis|0\n"
                                "mstate|0\n"
                                "smstate|0\n");

                ((player*)currentPeer->data)->send_packet(p);
            }
        }
    }
}
 */