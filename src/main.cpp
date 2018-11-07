#include "server/udp_server.h"

using namespace std;

int main() {
    auto server = udp_server(17091);
    growtopia::objects::init_itemdb();

    server.run(1024);
    return 0;
}