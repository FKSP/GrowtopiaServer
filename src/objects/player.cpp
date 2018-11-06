#include <utility>

#include "player.h"

player::player(ENetPeer *peer) {
    this->peer = peer;
}

void player::send_console_message(std::string message) {
    gamepacket* p1 = gamepacket::new_packet("OnConsoleMessage");
    p1->write(std::move(message));

    enet_peer_send(this->peer, 0, p1->get_enet_packet());

    delete p1;
}

bool player::is_loggedin(bool login) {
    if (login) this->loggedin = true;
    return this->loggedin;
}

void player::send_packet(gamepacket* packet) {
    enet_peer_send(this->peer, 0, packet->get_enet_packet());

    delete packet;
}

bool player::is_password(std::string password) {
    return false;
}

player::~player() {

}
