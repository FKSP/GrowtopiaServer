#ifndef GROWTOPIASERVER_LOGINHANDLER_H
#define GROWTOPIASERVER_LOGINHANDLER_H

#include "../helpers/login_parser.h"
#include "../packets/gamepacket.h"
#include "../objects/player.h"

void handle_login(player* player, std::string data);

#endif //GROWTOPIASERVER_LOGINHANDLER_H
