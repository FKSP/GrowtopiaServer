#ifndef GROWTOPIASERVER_PLAYER_H
#define GROWTOPIASERVER_PLAYER_H

#include <ostream>
#include <vector>

#include <enet/enet.h>

#include "../enums/block_types.h"
#include "../packets/gamepacket.h"
#include "../database/world_db.h"

namespace growtopia::objects {
    struct inventory_item {
        uint16_t item_id;
        uint8_t item_count;
    };
    struct inventory {
        std::vector<inventory_item> items;
        int inv_size = 100;
    };
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
    struct clothing_info {
        int hair = 0; // 0
        int shirt = 0; // 1
        int pants = 0; // 2
        int feet = 0; // 3
        int face = 0; // 4
        int hand = 0; // 5
        int back = 0; // 6
        int mask = 0; // 7
        int necklace = 0; // 8
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
        inventory inv;
        clothing_info clothing;
        int skin_color = 0x42;

        bool rotation_left = false;
        bool join_update_clothes = false;

        explicit player(ENetPeer* peer);

        bool is_loggedin(bool login = false);
        bool is_password(std::string password);

        void send_world_list();
        void send_world(database::world::world *world);
        void send_inventory();
        void send_console_message(std::string message);
        void send_items_db();
        void send_packet(gamepacket* packet);
        void send_raw_packet(int a1, void *packetData, size_t packetDataSize, void *a4, ENetPeer* peer, int packetFlag);
        void send_player_movement(player_movement& player_movement);
        void send_action(std::string action);
        void send_nothing_happend(int x, int y);
    private:
        bool loggedin = false;
    };

    void init_itemdb();
}
#endif //GROWTOPIASERVER_PLAYER_H
