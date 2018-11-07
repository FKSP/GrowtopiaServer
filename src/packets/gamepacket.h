#ifndef GROWTOPIASERVER_GAMEPACKET_H
#define GROWTOPIASERVER_GAMEPACKET_H

#include <stdint.h>
#include <string>

#include <enet/enet.h>

class gamepacket {
public:
    ~gamepacket();

    static gamepacket* new_packet(std::string packet_name);

    gamepacket* write(float val1, float val2, float val3);
    gamepacket* write(float val1, float val2);
    gamepacket* write(float val1);

    gamepacket* write(int val1, bool x = false);

    gamepacket* write(std::string val1);

    static uint8_t ch2n(int8_t x);

    ENetPacket* get_enet_packet();
private:
    uint8_t* data;
    size_t length;
    uint8_t indexes;

    gamepacket* begin_packet();
    gamepacket* end_packet();
};


#endif //GROWTOPIASERVER_GAMEPACKET_H
