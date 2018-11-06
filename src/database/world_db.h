#ifndef GROWTOPIASERVER_WORLD_DB_H
#define GROWTOPIASERVER_WORLD_DB_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <msgpack.hpp>
#include <vector>

#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

class world_block {
public:
    world_block() {
        this->foreground = 0;
        this->background = 0;
        this->break_level = 0;
        this->break_time = 0;
        this->break_sound = "default";
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
    std::string break_sound;

    MSGPACK_DEFINE(foreground, background, break_level, break_time, water, fire, glue, red, green, blue, break_sound);
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

    MSGPACK_DEFINE(name, blocks, width, height, joinable, blocks);

    void save();
};

class world_db {
public:
    world* get_world(std::string name);
    world* generate_world(std::string name, int width, int height);
};


#endif //GROWTOPIASERVER_WORLD_DB_H
