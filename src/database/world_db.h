#ifndef GROWTOPIASERVER_WORLD_DB_H
#define GROWTOPIASERVER_WORLD_DB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <enet/enet.h>
#include <msgpack.hpp>
#include <json.hpp>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>

#include "../helpers/random.h"
#include "../helpers/system.h"
#include "../helpers/string.h"
#include "../helpers/compression.h"
#include "../helpers/encoding.h"


namespace growtopia::database::world {
    enum ClothTypes {
        HAIR,
        SHIRT,
        PANTS,
        FEET,
        FACE,
        HAND,
        BACK,
        MASK,
        NECKLACE,
        NONE
    };

    enum BlockTypes {
        FOREGROUND,
        BACKGROUND,
        SEED,
        PAIN_BLOCK,
        BEDROCK,
        MAIN_DOOR,
        SIGN,
        DOOR,
        CLOTHING,
        FIST,
        UNKNOWN
    };
    struct ItemDefinition {
        int id;
        std::string name;
        int rarity;
        int breakHits;
        int growTime;
        ClothTypes clothType;
        BlockTypes blockType;
        std::string description = "This item has no description.";
    };
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
    };
    class world {
    public:
        world() {
            this->name = "NONE";
            this->width = 100, this->height = 60;
            this->joinable = true;
        }
        std::string name;
        int width, height;
        bool joinable;
        std::vector<world_block> blocks;

        int get_joined_peers(ENetHost *server);

        void save();
    };

    const world get_world(std::string name);
    const world generate_world(std::string name, int width, int height);
    const std::vector<world> get_random_worlds(int amount);
}

#endif //GROWTOPIASERVER_WORLD_DB_H
