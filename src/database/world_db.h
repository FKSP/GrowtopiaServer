#ifndef GROWTOPIASERVER_WORLD_DB_H
#define GROWTOPIASERVER_WORLD_DB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <enet/enet.h>
#include <msgpack.hpp>

#include "../helpers/random.h"
#include "../helpers/system.h"
#include "../helpers/string.h"

namespace growtopia::database::world {
    class world_block {
    public:
        world_block() {
            this->foreground = 0;
            this->background = 0;
            this->break_level = 0;
            this->break_time = 0;
            this->water = false;
            this->fire = false;
            this->glue = false;
            this->red = false;
            this->green = false;
            this->blue = false;
        }
        int16_t foreground;
        int16_t background;
        int break_level;
        long long int break_time;
        bool water;
        bool fire;
        bool glue;
        bool red;
        bool green;
        bool blue;

        int to_bitwise();
        MSGPACK_DEFINE(foreground, background, break_level, break_time, water, fire, glue, red, green, blue);
    };
    class world {
    public:
        world() {
            this->name = "NONE";
            this->width = 100, this->height = 50;
            this->joinable = true;
        }
        std::string name;
        int width, height;
        bool joinable;
        std::vector<world_block> blocks;

        int get_joined_peers(ENetHost *server);

        MSGPACK_DEFINE(name, blocks, width, height, joinable, blocks);

        void save();
    };

    const world get_world(std::string name);
    const world generate_world(std::string name, int width, int height);
    const std::vector<world> get_random_worlds(int amount);
}

#endif //GROWTOPIASERVER_WORLD_DB_H
