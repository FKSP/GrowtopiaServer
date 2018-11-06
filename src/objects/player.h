#ifndef GROWTOPIASERVER_PLAYER_H
#define GROWTOPIASERVER_PLAYER_H

#include <ostream>
#include <enet/enet.h>
#include "../packets/gamepacket.h"

class player {
public:
    ~player();

    std::string user_name;
    std::string display_name;
    std::string country;
    ENetPeer* peer;

    explicit player(ENetPeer* peer);

    bool is_loggedin(bool login = false);
    bool is_password(std::string password);

    void send_console_message(std::string message);
    void send_packet(gamepacket* packet);
private:
    bool loggedin = false;
};

#endif //GROWTOPIASERVER_PLAYER_H
