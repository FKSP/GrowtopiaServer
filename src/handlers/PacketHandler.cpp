#include "PacketHandler.h"

using namespace std;
using namespace growtopia;

unsigned long long client_id = 0;

void handle_packets(ENetHost* server, ENetPeer* peer, ENetPacket* packet, objects::player* player) {
    int type = get_packet_type(packet);
    gamepacket* p;
    string data;
    uint8_t* tank_data;
    stringstream ss;
    string to;
    bool is_joinreq = false;

    switch (type) {
        default:
            printf("Unknown type %d\n", type);
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

                    on_peer_connect(server, peer);

                    player->send_inventory();

                    player->client_id = static_cast<int>(client_id);
                    client_id++;
                }
            }
            break;
        case 4:
            tank_data = get_tankpacket(packet);
            if (tank_data)
            {
                auto movement = unpack_player_movements(tank_data);
                switch (movement->packet_type)
                {
                    case 0:
                        player->posx = movement->x;
                        player->posy = movement->y;
                        player->rotation_left = static_cast<bool>(movement->character_state & 0x10);

                        player->send_player_movement(*movement);

                        if (!player->join_update_clothes)
                        {
                            player->join_update_clothes = true;
                            update_all_clothes(server, player);
                        }
                        break;
                    default:
                        break;
                }

                movement = unpack_player_movements(tank_data);
                cout << movement->packet_type << ";" <<
                        movement->character_state << ";" <<
                        movement->punch_x << ";" <<
                        movement->punch_y << ";" <<
                        movement->x_speed << ";" <<
                        movement->y_speed << ";" <<
                        movement->x << ";" <<
                        movement->y << ";" <<
                        movement->planting_tree << ";" <<
                        movement->net_id << endl;

                if(movement->packet_type == 7)
                    player->send_world_list();

                if (movement->punch_x != -1 && movement->punch_y != -1) {
                    if (movement->packet_type == 3) {
                        //send_tile_update(movement->punch_x, movement->punch_y, movement->planting_tree,
                        //               player->client_id, peer);
                    }
                }
                player->send_nothing_happend(movement->x / 32, movement->y / 32);
            }
            break;
        case 2:
            data = get_text(packet);

            try {
                if (!player->is_loggedin())
                    return handlers::handle_login(player, data);
            } catch (int x) {
                if (x == 1)
                    return;
            }

            data = get_text(packet);
            if (data == "action|refresh_item_data\n")
                player->send_items_db();

            data = get_text(packet);
            if (data.substr(0, 17) == "action|enter_game" && player->is_loggedin()) {
                player->send_world_list();
                objects::inventory_item item{};
                item.item_count = 1;
                item.item_id = 18;
                player->inv.items.push_back(item);

                p = gamepacket::new_packet("OnDialogRequest")
                        ->write("set_default_color|`o\n\nadd_label_with_icon|big|`wThe Growtopia Gazette``|left|5016|\n\nadd_spacer|small|\nadd_label_with_icon|small|`4WARNING:`` `5Worlds (and accounts)`` might be deleted at any time if database issues appear (once per day or week).|left|4|\nadd_label_with_icon|small|`4WARNING:`` `5Accounts`` are in beta, bugs may appear and they will be probably deleted often, because of new account updates, which will cause database incompatibility.|left|4|\nadd_spacer|small|\n\nadd_url_button||``Watch: `1Watch a video about GT Private Server``|NOFLAGS|https://www.youtube.com/watch?v=_3avlDDYBBY|Open link?|0|0|\nadd_url_button||``Channel: `1Watch Growtopia Noobs' channel``|NOFLAGS|https://www.youtube.com/channel/UCLXtuoBlrXFDRtFU8vPy35g|Open link?|0|0|\nadd_url_button||``Items: `1Item database by Nenkai``|NOFLAGS|https://raw.githubusercontent.com/Nenkai/GrowtopiaItemDatabase/master/GrowtopiaItemDatabase/CoreData.txt|Open link?|0|0|\nadd_url_button||``Discord: `1GT Private Server Discord``|NOFLAGS|https://discord.gg/8WUTs4v|Open the link?|0|0|\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|");

                player->send_packet(p);
            }

            data = get_text(packet);
            cout << data << endl;
            break;
    }
    enet_packet_destroy(packet);
}

void update_all_clothes(ENetHost* server, objects::player* p) {
    ENetPeer* currentPeer;
    for (currentPeer = server->peers;
         currentPeer < &server->peers[server->peerCount];
         ++currentPeer) {
        if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
            continue;
        if (is_here(p->peer, currentPeer))
        {
            gamepacket* pack = gamepacket::new_packet("OnSetClothing")
                    ->write(p->clothing.hair, p->clothing.shirt, p->clothing.pants)
                    ->write(p->clothing.feet, p->clothing.face, p->clothing.hand)
                    ->write(p->clothing.back, p->clothing.mask, p->clothing.necklace)
                    ->write(p->skin_color, true)
                    ->write(0.0f, 0.0f, 0.0f);

            ((objects::player*)currentPeer->data)->send_packet(pack);

            pack = gamepacket::new_packet("OnSetClothing")
                    ->write(((objects::player*)currentPeer->data)->clothing.hair, ((objects::player*)currentPeer->data)->clothing.shirt, ((objects::player*)currentPeer->data)->clothing.pants)
                    ->write(((objects::player*)currentPeer->data)->clothing.feet, ((objects::player*)currentPeer->data)->clothing.face, ((objects::player*)currentPeer->data)->clothing.hand)
                    ->write(((objects::player*)currentPeer->data)->clothing.back, ((objects::player*)currentPeer->data)->clothing.mask, ((objects::player*)currentPeer->data)->clothing.necklace)
                    ->write(((objects::player*)currentPeer->data)->skin_color, true)
                    ->write(0.0f, 0.0f, 0.0f);

            p->send_packet(pack);
        }
    }
}

int get_packet_type(ENetPacket *packet) {
    int result = 0;

    if (packet->dataLength > 3u)
        result = *(packet->data);
    else
        printf("Bad packet length, ignoring packet\n");

    return result;
}

char* get_text(ENetPacket* packet) {
    char zero = 0;
    memcpy(packet->data + packet->dataLength - 1, &zero, 1);
    return (char*)(packet->data + 4);
}

uint8_t* get_tankpacket(ENetPacket* packet) {
    auto packet_length = static_cast<unsigned int>(packet->dataLength);
    uint8_t* result = nullptr;
    if (packet_length >= 0x3C)
    {
        uint8_t* packetData = packet->data;
        result = packetData + 4;
        if (*(packetData + 16) & 8)
        {
            if (packet_length < *(int*)(packetData + 56) + 60)
            {
                cout << "Packet too small for extended packet to be valid" << endl;
                cout << "Sizeof float is 4.  TankUpdatePacket size: 56" << endl;
                result = 0;
            }
        }
        else
        {
            int zero = 0;
            memcpy(packetData + 56, &zero, 4);
        }
    }
    return result;
}

bool is_here(ENetPeer* peer, ENetPeer* peer2) {
    return ((objects::player*)(peer->data))->joined_world == ((objects::player*)(peer2->data))->joined_world;
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
                string netIdS = std::to_string(((objects::player*)currentPeer->data)->client_id);
                gamepacket* p = gamepacket::new_packet("OnSpawn")
                        ->write("spawn|avatar\n"
                                "netID|" + netIdS + "\n"
                                "userID|" + netIdS + "\n"
                                "colrect|0|0|20|30\n"
                                "posXY|" + std::to_string(((objects::player*)currentPeer->data)->posx) + "|" + std::to_string(((objects::player*)currentPeer->data)->posy) + "\n"
                                "name|``" + ((objects::player*)currentPeer->data)->display_name + "``\n"
                                "country|" + ((objects::player*)currentPeer->data)->country + "\n"
                                "invis|0\n"
                                "mstate|0\n"
                                "smstate|0\n");

                ((objects::player*)peer->data)->send_packet(p);

                string netIdS2 = std::to_string(((objects::player*)(peer->data))->client_id);
                p = gamepacket::new_packet("OnSpawn")
                        ->write("spawn|avatar\n"
                                "netID|" + netIdS2 + "\n"
                                "userID|" + netIdS2 + "\n"
                                "colrect|0|0|20|30\n"
                                "posXY|" + std::to_string(((objects::player*)peer->data)->posx) + "|" + std::to_string(((objects::player*)peer->data)->posy) + "\n"
                                "name|``" + ((objects::player*)peer->data)->display_name + "``\n"
                                "country|" + ((objects::player*)peer->data)->country + "\n"
                                "invis|0\n"
                                "mstate|0\n"
                                "smstate|0\n");

                ((objects::player*)currentPeer->data)->send_packet(p);
            }
        }
    }
}

objects::player_movement* unpack_player_movements(uint8_t* data) {
    auto dataStruct = new objects::player_movement;
    memcpy(&dataStruct->packet_type, data, 4);
    memcpy(&dataStruct->net_id, data + 4, 4);
    memcpy(&dataStruct->character_state, data + 12, 4);
    memcpy(&dataStruct->planting_tree, data + 20, 4);
    memcpy(&dataStruct->x, data + 24, 4);
    memcpy(&dataStruct->y, data + 28, 4);
    memcpy(&dataStruct->x_speed, data + 32, 4);
    memcpy(&dataStruct->y_speed, data + 36, 4);
    memcpy(&dataStruct->punch_x, data + 44, 4);
    memcpy(&dataStruct->punch_y, data + 48, 4);
    return dataStruct;
}

void send_tile_update(int x, int y, int tile, objects::player* player) {
    objects::player_movement data{};
    //data.packetType = 0x14;
    data.packet_type = 0x3;

    //data.characterState = 0x924; // animation
    data.character_state = 0x0; // animation
    data.x = x;
    data.y = y;
    data.punch_x = x;
    data.punch_y = y;
    data.x_speed = 0;
    data.y_speed = 0;
    data.net_id = player->client_id;
    data.planting_tree = tile;

    database::world::world wlrd = database::world::get_world(player->joined_world);
    database::world::world* world = &wlrd;

    if (x<0 || y<0 || x>world->width || y>world->height) return;

    player->send_nothing_happend(x/32, y/32);

    if (world->blocks[x + (y*world->width)].foreground == 6 || world->blocks[x + (y*world->width)].foreground == 8 || world->blocks[x + (y*world->width)].foreground == 3760)
        return;
    if (tile == 6 || tile == 8 || tile == 3760)
        return;
    /*
    if (world->owner != "") {
        if (((PlayerInfo*)(peer->data))->rawName == world->owner) {
            // WE ARE GOOD TO GO
            if (tile == 32) {
                GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnDialogRequest"), "set_default_color|`o\n\nadd_label_with_icon|big|`wShould this world be publicly breakable?``|left|242|\n\nadd_spacer|small|\nadd_button_with_icon|worldPublic|Public|noflags|2408||\nadd_button_with_icon|worldPrivate|Private|noflags|202||\nadd_spacer|small|\nadd_quick_exit|\nadd_button|chc0|Close|noflags|0|0|\nnend_dialog|gazette||OK|"));
                ENetPacket * packet = enet_packet_create(p.data,
                                                         p.len,
                                                         ENET_PACKET_FLAG_RELIABLE);
                enet_peer_send(peer, 0, packet);
                delete p.data;
            }
        }
        else if (world->isPublic)
        {
            if (world->items[x + (y*world->width)].foreground == 242)
            {
                return;
            }
        }
        else {
            return;
        }
        if (tile == 242) {
            return;
        }

    }
             */
    if (tile == 32) {
        // TODO
        return;
    }
    if (tile == 822) {
        world->blocks[x + (y*world->width)].water = !world->blocks[x + (y*world->width)].water;
        return;
    }
    if (tile == 3062)
    {
        world->blocks[x + (y*world->width)].fire = !world->blocks[x + (y*world->width)].fire;
        return;
    }
    if (tile == 1866)
    {
        world->blocks[x + (y*world->width)].glue = !world->blocks[x + (y*world->width)].glue;
        return;
    }
    database::world::ItemDefinition def;
    /*
    try {
        def = getItemDef(tile);
        if (def.clothType != ClothTypes::NONE) return;
    }
    catch (int e) {
        def.breakHits = 4;
        def.blockType = BlockTypes::UNKNOWN;
        cout << "Ugh, unsupported item " << tile << endl;
    }
     */

    if (tile == 544 || tile == 546 || tile == 4520 || tile == 382 || tile == 3116 || tile == 4520 || tile == 1792 || tile == 5666 || tile==2994 || tile==4368) return;
    if (tile == 5708 || tile == 5709 || tile == 5780 || tile == 5781 || tile == 5782 || tile == 5783 || tile == 5784 || tile == 5785 || tile == 5710 || tile == 5711 || tile == 5786 || tile == 5787 || tile == 5788 || tile == 5789 || tile == 5790 || tile == 5791 || tile == 6146 || tile == 6147 || tile == 6148 || tile == 6149 || tile == 6150 || tile == 6151 || tile == 6152 || tile == 6153 || tile == 5670 || tile == 5671 || tile == 5798 || tile == 5799 || tile == 5800 || tile == 5801 || tile == 5802 || tile == 5803 || tile == 5668 || tile == 5669 || tile == 5792 || tile == 5793 || tile == 5794 || tile == 5795 || tile == 5796 || tile == 5797 || tile == 544 || tile == 546 || tile == 4520 || tile == 382 || tile == 3116 || tile == 1792 || tile == 5666 || tile == 2994 || tile == 4368) return;
    if(tile == 1902 || tile == 1508 || tile == 428) return;
    if (tile == 410 || tile == 1770 || tile == 4720 || tile == 4882 || tile == 6392 || tile == 3212 || tile == 1832 || tile == 4742 || tile == 3496 || tile == 3270 || tile == 4722) return;
    if (tile >= 7068) return;
    if (tile == 0 || tile == 18) {
        data.packet_type = 0x8;
        data.planting_tree = 4;

        if (y < world->height && world->blocks[x + (y*world->width)].break_level + 4 >= def.breakHits * 4) { // TODO
            data.packet_type = 0x3;// 0xC; // 0xF // World::HandlePacketTileChangeRequest
            data.net_id = -1;
            data.planting_tree = 0;
            world->blocks[x + (y*world->width)].break_level = 0;
            if (world->blocks[x + (y*world->width)].foreground != 0)
            {
                world->blocks[x + (y*world->width)].foreground = 0;
            }
            else {
                world->blocks[x + (y*world->width)].background = 0;
            }

        }
    }
    else {
        /*
        for (int i = 0; i < player->inv.items.size(); i++)
        {
            if (player->inv.items.at(i).item_id == tile)
            {
                if ((unsigned int)player->inv.items.at(i).item_count>1)
                    player->inv.items.at(i).item_count--;
                else
                    player->inv.items.erase(player->inv.items.begin() + i);

            }
        }
        if (def.blockType == world::BlockTypes::BACKGROUND)
        {
            world->items[x + (y*world->width)].background = tile;
        }
        else {
            world->items[x + (y*world->width)].foreground = tile;
            if (tile == 242) {
                world->owner = player->user_name;
                world->isPublic = false;
                ENetPeer * currentPeer;

                for (currentPeer = player->peer->host->peers;
                     currentPeer < &player->peer->host->peers[player->peer->host->peerCount];
                     ++currentPeer)
                {
                    if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
                        continue;
                    if (is_here(player->peer->host, player->peer, currentPeer)) {

                        GamePacket p = packetEnd(appendString(appendString(createPacket(), "OnConsoleMessage"), "`3[`w" + world->name + " `ohas been World Locked by `2" + ((PlayerInfo*)(peer->data))->displayName + "`3]"));
                        ENetPacket * packet = enet_packet_create(p.data,
                                                                 p.len,
                                                                 ENET_PACKET_FLAG_RELIABLE);
                        enet_peer_send(currentPeer, 0, packet);
                        delete p.data;

                    }
                }
            }

        }
         */
        world->blocks[x + (y*world->width)].break_level = 0;
    }

    ENetPeer* currentPeer;

    for (currentPeer = player->peer->host->peers;
         currentPeer < &player->peer->host->peers[player->peer->host->peerCount];
         ++currentPeer)
    {
        if (currentPeer->state != ENET_PEER_STATE_CONNECTED)
            continue;
        //if (is_here(player->peer, currentPeer))
            //((*player)currentPeer).send_player_movement(data);

        //cout << "Tile update at: " << data2->punchX << "x" << data2->punchY << endl;
    }
}
