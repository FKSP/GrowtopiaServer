#ifndef GROWTOPIASERVER_PACKETHANDLER_H
#define GROWTOPIASERVER_PACKETHANDLER_H

#include <iostream>
#include <fstream>
#include <enet/enet.h>
#include "../objects/player.h"
#include "../helpers/login_parser.h"
#include "../packets/gamepacket.h"

#include "LoginHandler.h"

void handle_packets(ENetPeer* peer, ENetPacket* packet, player* player);
int get_packet_type(ENetPacket* packet);
char* get_text(ENetPacket* packet);

#endif //GROWTOPIASERVER_PACKETHANDLER_H
