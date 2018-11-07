#include "world_db.h"

using namespace std;
using namespace growtopia;
using namespace database;

vector<string> worlds;

const world::world world::get_world(string name) {
    world w;
    name = helpers::get_upper(name);
    
    mkdir("worlds");

    ifstream world_file("worlds/" + name + ".wrld", ios::binary | ios::ate);

    if (!world_file.good()) {
        w = generate_world(name, 100, 50);
        w.save();
    } else {
        ifstream::pos_type pos = world_file.tellg();
        auto length = static_cast<size_t>(pos);
        char *worldData = new char[length];
        world_file.seekg(0, ios::beg);
        world_file.read(worldData, length);

        msgpack::object_handle oh = msgpack::unpack(worldData, length);

        msgpack::object obj = oh.get();
        obj.convert(w);
    }

    worlds.push_back(w.name);

    world_file.close();
    return w;
}

const world::world world::generate_world(string name, int width, int height) {
    world w;

    w.name = move(name);
    w.width = width;
    w.height = height;

    for (int i = 0; i < w.width*w.height; i++)
    {
        w.blocks.emplace_back();
        if (i >= 3800 && i<5400 && !(helpers::random_number(0, 100) % 50))
            w.blocks[i].foreground = 10;
        else if (i >= 3700 && i<5400)
            w.blocks[i].foreground = 2;
        else if (i >= 5400)
            w.blocks[i].foreground = 8;
        if (i >= 3700)
            w.blocks[i].background = 14;
        if (i == 3650)
            w.blocks[i].foreground = 6;
        else if (i >= 3600 && i<3700)
            w.blocks[i].foreground = 16;
        if (i == 3750)
            w.blocks[i].foreground = 8;
    }

    return w;
}

void world::world::save() {
    ofstream out("worlds/" + helpers::get_upper(name) + ".wrld", istream::out | ios::binary);

    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(&buffer);

    pk.pack(*this);

    out.write(buffer.data(), buffer.size());
    out.close();
    buffer.clear();
}

int world::world::get_joined_peers(ENetHost *server) {
    return 0;
}

const vector<world::world> world::get_random_worlds(int amount) {
    auto vec = vector<world>();

    if (worlds.empty())
        get_world("start");

    string x;
    int fail = 0;

    for (int i = 0; i < amount; ++i) {
        if (i > worlds.size()) break;
        try {
            if (fail > 5) break;
            string world_name = worlds[helpers::random_number(0, (int)worlds.size() -1)];
            if (string(x).find(world_name) != string::npos) {
                i--;
                fail++;
                continue;
            }
            x += world_name + " ";
            vec.push_back(get_world(world_name));
        } catch (bad_alloc&) {
            cout << "bad_alloc" << endl;
            worlds.erase(worlds.begin() + i);
        }
    }

    if (vec.empty())
        vec.push_back(get_world("start"));

    return vec;
}
