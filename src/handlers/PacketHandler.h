#ifndef GROWTOPIASERVER_PACKETHANDLER_H
#define GROWTOPIASERVER_PACKETHANDLER_H

#include <iostream>
#include <fstream>
#include <enet/enet.h>
#include "../objects/player.h"
#include "../helpers/login_parser.h"
#include "../packets/gamepacket.h"

#include "login_handler.h"

bool is_here(ENetPeer* peer, ENetPeer* peer2);
void on_peer_connect(ENetHost* server, ENetPeer* peer);
void handle_packets(ENetHost* server, ENetPeer* peer, ENetPacket* packet, growtopia::objects::player* player);
int get_packet_type(ENetPacket* packet);
char* get_text(ENetPacket* packet);
uint8_t* get_tankpacket(ENetPacket* packet);
growtopia::objects::player_movement* unpack_player_movements(uint8_t* data);
void update_all_clothes(ENetHost* server, growtopia::objects::player* p);

#endif //GROWTOPIASERVER_PACKETHANDLER_H
