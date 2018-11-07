#ifndef GROWTOPIASERVER_LOGINHANDLER_H
#define GROWTOPIASERVER_LOGINHANDLER_H

#include "../helpers/login_parser.h"
#include "../packets/gamepacket.h"
#include "../objects/player.h"

namespace growtopia::handlers {
    void handle_login(growtopia::objects::player* player, std::string data);
}

#endif //GROWTOPIASERVER_LOGINHANDLER_H
