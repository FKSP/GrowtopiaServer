#include "world_db.h"

world* world_db::get_world(std::string name) {

#if WIN32
    _mkdir("worlds");
#else
    mkdir("worlds");
#endif

    auto w = new world;
    std::ifstream wrld("worlds/" + name + ".wrld");

    if (!wrld.good()) {
        w = generate_world(name, 100, 50);
        w->save();
    } else {
        std::string str((std::istreambuf_iterator<char>(wrld)),
                        std::istreambuf_iterator<char>());

        msgpack::object_handle oh = msgpack::unpack(str.c_str(), str.size());

        msgpack::object obj = oh.get();
        world tmp;
        obj.convert(tmp);
        w = &tmp;
    }

    wrld.close();
    return w;
}

world* world_db::generate_world(std::string name, int width, int height) {
    auto w = new world;

    w->name = std::move(name);
    w->width = width;
    w->height = height;

    for (int i = 0; i < w->width*w->height; i++)
    {
        w->blocks.emplace_back(world_block());
        if (i >= 3800 && i<5400 && !(rand() % 50))
            w->blocks[i].foreground = 10;
        else if (i >= 3700 && i<5400)
            w->blocks[i].foreground = 2;
        else if (i >= 5400)
            w->blocks[i].foreground = 8;
        if (i >= 3700)
            w->blocks[i].background = 14;
        if (i == 3650)
            w->blocks[i].foreground = 6;
        else if (i >= 3600 && i<3700)
            w->blocks[i].foreground = 16;
        if (i == 3750)
            w->blocks[i].foreground = 8;
    }

    return w;
}

void world::save() {
    std::ofstream out("worlds/" + this->name + ".wrld");
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(*this);
    out.write(buffer.data(), buffer.size());
    buffer.clear();
    out.close();
}
