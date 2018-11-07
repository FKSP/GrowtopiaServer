#include <utility>

#include "player.h"

using namespace std;
using namespace growtopia;

uint8_t* itemsDat = nullptr;
int itemsDatSize = 0;

void objects::init_itemdb() {
    ifstream file("items.dat", ios::binary | ios::ate);
    if (!file.good()) return;

    itemsDatSize = static_cast<int>(file.tellg());

    itemsDat = new BYTE[60 + itemsDatSize];
    string raw_data = "0400000010000000FFFFFFFF000000000800000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    for (int i = 0; i < raw_data.length(); i += 2)
    {
        char x = gamepacket::ch2n(raw_data[i]);
        x = x << 4;
        x += gamepacket::ch2n(raw_data[i + 1]);
        memcpy(itemsDat + (i / 2), &x, 1);
        if (raw_data.length() > 60 * 2) throw 0;
    }
    memcpy(itemsDat + 56, &itemsDatSize, 4);
    file.seekg(0, ios::beg);

    if (file.read((char*)(itemsDat + 60), itemsDatSize))
        cout << "Updating item data success!" << endl;
    else
        cout << "Updating item data failed!" << endl;
}

objects::player::player(ENetPeer *peer) {
    this->peer = peer;
}

bool objects::player::is_loggedin(bool login) {
    if (login) this->loggedin = true;
    return this->loggedin;
}

void objects::player::send_console_message(string message) {
    gamepacket* p = gamepacket::new_packet("OnConsoleMessage")
            ->write(move(message));
    this->send_packet(p);
}

void objects::player::send_world_list() {
    if (!is_loggedin()) return;

    vector<database::world::world> worlds = database::world::get_random_worlds(12);
    string worldOffers = "default|";
    if (!worlds.empty())
        worldOffers += worlds[0].name;

    worldOffers += "\nadd_button|Showing: `wWorlds``|_catselect_|0.6|3529161471|\n";
    for (auto world : worlds)
        worldOffers += "add_floater|"+ world.name+"|"+to_string(world.get_joined_peers(peer->host))+"|"+to_string(helpers::random_number(.4f, .6f))+"|3529161471\n";


    gamepacket* p = gamepacket::new_packet("OnRequestWorldSelectMenu")
            ->write(worldOffers);
    this->send_packet(p);
}

void objects::player::send_items_db() {
    if (itemsDat != nullptr) {
        this->is_updating = true;
        ENetPacket *packet = enet_packet_create(itemsDat,
                                                static_cast<size_t>(itemsDatSize) + 60,
                                                ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
        enet_peer_disconnect_later(peer, 0);
    }
}

void objects::player::send_packet(gamepacket* packet) {
    enet_peer_send(this->peer, 0, packet->get_enet_packet());
    delete packet;
}

bool objects::player::is_password(string password) {
    return false;
}

void objects::player::send_world(database::world::world *world) {
    //this->joinClothesUpdated = false;
    string raw_data = "0400000004A7379237BB2509E8E0EC04F8720B050000000000000000FBBB0000010000007D920100FDFDFDFD04000000040000000000000000000000070000000000";
    string worldName = world->name;
    int xSize = world->width;
    int ySize = world->height;
    int square = xSize*ySize;
    auto nameLen = static_cast<short>(worldName.length());
    int payloadLen = static_cast<int>(raw_data.length() / 2);
    int dataLen = payloadLen + 2 + nameLen + 12 + (square * 8) + 4;
    int allocMem = payloadLen + 2 + nameLen + 12 + (square * 8) + 4 + 16000;
    auto data = new uint8_t[allocMem];
    int zero = 0;
    __int16 item = 0;
    int smth = 0;

    for (int i = 0; i < raw_data.length(); i += 2) {
        char x = gamepacket::ch2n(raw_data[i]);
        x = x << 4;
        x += gamepacket::ch2n(raw_data[i + 1]);
        memcpy(data + (i / 2), &x, 1);
    }

    for (int i = 0; i < square * 8; i += 4) memcpy(data + payloadLen + i + 14 + nameLen, &zero, 4);
    for (int i = 0; i < square * 8; i += 8) memcpy(data + payloadLen + i + 14 + nameLen, &item, 2);

    memcpy(data + payloadLen, &nameLen, 2);
    memcpy(data + payloadLen + 2, worldName.c_str(), nameLen);
    memcpy(data + payloadLen + 2 + nameLen, &xSize, 4);
    memcpy(data + payloadLen + 6 + nameLen, &ySize, 4);
    memcpy(data + payloadLen + 10 + nameLen, &square, 4);

    uint8_t* blockPtr = data + payloadLen + 14 + nameLen;

    for (int i = 0; i < square; i++) {
        if ((world->blocks[i].foreground == 0) || (world->blocks[i].foreground == 2) || (world->blocks[i].foreground == 8) || (world->blocks[i].foreground == 100)) {
            memcpy(blockPtr, &world->blocks[i].foreground, 2);
            int type = enums::block_types::none;
            // type 1 = locked
            if (world->blocks[i].water)
                type |= enums::block_types::water;
            if (world->blocks[i].glue)
                type |= enums::block_types::glue;
            if (world->blocks[i].fire)
                type |= enums::block_types::fire;
            if (world->blocks[i].red)
                type |= enums::block_types::color_red;
            if (world->blocks[i].green)
                type |= enums::block_types::color_green;
            if (world->blocks[i].blue)
                type |= enums::block_types::color_blue;
            memcpy(blockPtr + 4, &type, 4);
        } else
            memcpy(blockPtr, &zero, 2);
        memcpy(blockPtr + 2, &world->blocks[i].background, 2);
        blockPtr += 8;
    }
    memcpy(data + dataLen - 4, &smth, 4);
    ENetPacket * packet2 = enet_packet_create(data,
                                              dataLen,
                                              ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(this->peer, 0, packet2);
    for (int i = 0; i < square; i++) {
        if ((world->blocks[i].foreground == 0) || (world->blocks[i].foreground == 8) || (world->blocks[i].foreground == 100))
            ; // nothing
        else
        {
            player_movement player_data{};
            //data.packetType = 0x14;
            player_data.packet_type = 0x3;

            //data.characterState = 0x924; // animation
            player_data.character_state = 0x0; // animation
            player_data.x = i%world->width;
            player_data.y = i/world->height;
            player_data.punch_x = i%world->width;
            player_data.punch_y = i / world->width;
            player_data.x_speed = 0;
            player_data.y_speed = 0;
            player_data.net_id = -1;
            player_data.planting_tree = world->blocks[i].foreground;

            this->send_player_movement(player_data);
        }
    }
    this->joined_world = world->name;
    //delete data;
}

void objects::player::send_player_movement(player_movement& player_movement) {
    auto data = new uint8_t[56];
    for (int i = 0; i < 56; i++)
        data[i] = 0;
    memcpy(data, &player_movement.packet_type, 4);
    memcpy(data + 4, &player_movement.net_id, 4);
    memcpy(data + 12, &player_movement.character_state, 4);
    memcpy(data + 20, &player_movement.planting_tree, 4);
    memcpy(data + 24, &player_movement.x, 4);
    memcpy(data + 28, &player_movement.y, 4);
    memcpy(data + 32, &player_movement.x_speed, 4);
    memcpy(data + 36, &player_movement.y_speed, 4);
    memcpy(data + 44, &player_movement.punch_x, 4);
    memcpy(data + 48, &player_movement.punch_y, 4);

    this->send_raw_packet(4, data, 56, 0, peer, ENET_PACKET_FLAG_RELIABLE);
}

void objects::player::send_raw_packet(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag) {
    ENetPacket *p;

    if (peer)
    {
        if (a1 == 4 && *((uint8_t *)packetData + 12) & 8)
        {
            p = enet_packet_create(nullptr, packetDataSize + *((uint64_t *)packetData + 13) + 5, packetFlag);
            int four = 4;
            memcpy(p->data, &four, 4);
            memcpy((char *)p->data + 4, packetData, packetDataSize);
            memcpy((char *)p->data + packetDataSize + 4, a4, *((uint64_t *)packetData + 13));
            enet_peer_send(peer, 0, p);
        }
        else
        {
            p = enet_packet_create(0, packetDataSize + 5, packetFlag);
            memcpy(p->data, &a1, 4);
            memcpy((char *)p->data + 4, packetData, packetDataSize);
            enet_peer_send(peer, 0, p);
        }
    }
    delete packetData;
}