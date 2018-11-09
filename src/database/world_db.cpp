#include "world_db.h"

using namespace std;
using namespace growtopia;
using namespace database;
using json = nlohmann::json;

vector<string> worlds;

const world::world world::get_world(string name) {
    world w;
    name = helpers::get_upper(name);
    
    helpers::makedir("worlds");

    ifstream world_file("worlds/" + name + ".wrld", ios::binary);


    if (!world_file.good()) {
        w = generate_world(name, 100, 60);
        w.save();
    } else {
        std::string data((std::istreambuf_iterator<char>(world_file)),
                          std::istreambuf_iterator<char>());
        std::stringstream compressed;
        std::stringstream decompressed;
        std::string decompressed_data;

        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::zlib_decompressor());
        in.push(decompressed_data);
        boost::iostreams::copy(in, compressed);

        compressed << data;

        cout << decompressed_data << endl;

        json j = json::parse(decompressed_data);

        cout << j;


        /*
        msgpack::object_handle oh;

        oh.get().convert(w.name);
        oh.get().convert(w.width);
        oh.get().convert(w.height);

        while(pac.next(oh)) {
            world_block block;
            oh.get().convert(block.foreground);
            oh.get().convert(block.background);
            oh.get().convert(block.water);
            oh.get().convert(block.fire);
            oh.get().convert(block.glue);
            oh.get().convert(block.red);
            oh.get().convert(block.green);
            oh.get().convert(block.blue);
            w.blocks.push_back(block);
        }
         */
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
    ofstream out("worlds/" + helpers::get_upper(name) + ".wrld", ios::binary);

    msgpack::sbuffer buffer;

    json blocks_array = json::array();

    for (auto block : this->blocks) {
        json jblock {
                {"foreground", block.foreground},
                {"background", block.background},
                {"background", block.water},
                {"background", block.fire},
                {"background", block.glue},
                {"background", block.red},
                {"background", block.green},
                {"background", block.blue},
        };
        blocks_array.push_back(jblock);
    }

    json j {
            {"name", this->name},
            {"width", this->width},
            {"height", this->height},
            {"joinable", this->joinable},
            {"blocks", blocks_array}
    };

    boost::iostreams::filtering_ostream os = boost::iostreams::filtering_ostream();
    os.push(boost::iostreams::zlib_compressor());

    string x = j.dump();

    os.push(*out.rdbuf());

    os << j;

    os.pop();
    out.close();
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
