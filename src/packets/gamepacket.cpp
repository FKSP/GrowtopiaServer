#include <utility>

#include "gamepacket.h"

using namespace std;

gamepacket::~gamepacket() {
    delete this->data;
}

gamepacket* gamepacket::begin_packet() {
    auto* data = new uint8_t[61];
    string raw_packet = "0400000001000000FFFFFFFF00000000080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    for (int i = 0; i < raw_packet.length(); i += 2)
    {
        char x = ch2n(raw_packet[i]);
        x = x << 4;
        x += ch2n(raw_packet[i + 1]);
        memcpy(data + (i / 2), &x, 1);
        if (raw_packet.length() > 61 * 2) throw 0;
    }
    this->data = data;
    this->length = 61;
    this->indexes = 0;
    return this;
}

gamepacket* gamepacket::write(float val1, float val2, float val3) {
    if (this->data == nullptr) this->begin_packet();
    auto* n = new uint8_t[this->length + 2 + 12];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    n[this->length] = this->indexes;
    n[this->length + 1] = 4;
    memcpy(n + this->length + 2, &val1, 4);
    memcpy(n + this->length + 6, &val2, 4);
    memcpy(n + this->length + 10, &val3, 4);
    this->length = this->length + 2 + 12;
    this->indexes++;
    return this;
}
gamepacket* gamepacket::write(float val1, float val2) {
    if (this->data == nullptr) this->begin_packet();
    auto* n = new uint8_t[this->length + 2 + 8];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    n[this->length] = this->indexes;
    n[this->length + 1] = 3;
    memcpy(n + this->length + 2, &val1, 4);
    memcpy(n + this->length + 6, &val2, 4);
    this->length = this->length + 2 + 8;
    this->indexes++;
    return this;
}
gamepacket* gamepacket::write(float val1) {
    if (this->data == nullptr) this->begin_packet();
    auto* n = new uint8_t[this->length + 2 + 4];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    n[this->length] = this->indexes;
    n[this->length + 1] = 9;
    memcpy(n + this->length + 2, &val1, 4);
    this->length = this->length + 2 + 4;
    this->indexes++;
    return this;
}

gamepacket* gamepacket::write(int val1, bool x) {
    if (this->data == nullptr) this->begin_packet();
    auto* n = new uint8_t[this->length + 2 + 4];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    n[this->length] = this->indexes;

    if (!x)
        n[this->length + 1] = 9;
    else
        n[this->length + 1] = 5;

    memcpy(n + this->length + 2, &val1, 4);
    this->length = this->length + 2 + 4;
    this->indexes++;
    return this;
}

gamepacket* gamepacket::write(std::string val1) {
    if (this->data == nullptr) this->begin_packet();
    auto* n = new uint8_t[this->length + 2 + val1.length() + 4];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    n[this->length] = this->indexes;
    n[ this->length + 1] = 2;
    size_t sLen = val1.length();
    memcpy(n + this->length + 2, &sLen, 4);
    memcpy(n + this->length + 6, val1.c_str(), sLen);
    this->length = this->length + 2 + val1.length() + 4;
    this->indexes++;

    return this;
}

gamepacket* gamepacket::end_packet() {
    auto* n = new uint8_t[this->length + 1];
    memcpy(n, this->data, this->length);
    delete this->data;
    this->data = n;
    char zero = 0;
    memcpy(this->data+this->length, &zero, 1);
    this->length += 1;
    *(int*)(this->data + 56) = this->indexes;
    *(this->data + 60) = this->indexes;
    return this;
}

uint8_t gamepacket::ch2n(int8_t x) {
    switch (x)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
        default:
            return 0;
    }
}

ENetPacket* gamepacket::get_enet_packet() {
    this->end_packet();
    return enet_packet_create(
            this->data,
            this->length,
            ENET_PACKET_FLAG_RELIABLE);
}

gamepacket* gamepacket::new_packet(string packet_name) {
    return (new gamepacket())->write(std::move(packet_name));
}

