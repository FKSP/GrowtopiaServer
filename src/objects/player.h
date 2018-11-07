#ifndef GROWTOPIASERVER_PLAYER_H
#define GROWTOPIASERVER_PLAYER_H

#include <ostream>
#include <vector>

#include <enet/enet.h>

#include "../enums/block_types.h"
#include "../packets/gamepacket.h"
#include "../database/world_db.h"

namespace growtopia::objects {
    struct player_movement {
        int packet_type;
        int net_id;
        float x;
        float y;
        int character_state;
        int planting_tree;
        float x_speed;
        float y_speed;
        int punch_x;
        int punch_y;
    };

    class player {
    public:
        std::string user_name;
        std::string display_name;
        std::string country;
        ENetPeer* peer;
        std::string joined_world;
        bool is_updating = false;
        int client_id;

        float posx;
        float posy;

        explicit player(ENetPeer* peer);

        bool is_loggedin(bool login = false);
        bool is_password(std::string password);

        void send_world_list();
        void send_world(database::world::world *world);
        void send_console_message(std::string message);
        void send_items_db();
        void send_packet(gamepacket* packet);
        void send_raw_packet(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag);
        void send_player_movement(player_movement& player_movement);
    private:
        bool loggedin = false;
    };

    void init_itemdb();
    bool is_here(ENetPeer* peer, ENetPeer* peer2);
    void on_peer_connect(ENetHost* server, ENetPeer* peer);
}
#endif //GROWTOPIASERVER_PLAYER_H
