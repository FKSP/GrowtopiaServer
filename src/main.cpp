#include <iostream>

#include "server/udp_server.h"
#include "database/world_db.h"

using namespace std;

int main() {
    udp_server server(17091);
    server.run(1024);
    return 0;
}