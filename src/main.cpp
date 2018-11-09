#include "server/udp_server.h"

using namespace std;

void cleanup(int s){
    printf("Caught signal %d\n",s);
    enet_deinitialize();
    exit(1);
}

#if WIN32
#include <windows.h>
#include <stdio.h>

BOOL WINAPI consoleHandler(DWORD signal) {
    cleanup(0);
    return true;
}
#else
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#endif

int main() {
    auto server = udp_server(27015); //17091
    growtopia::objects::init_itemdb();

#if WIN32
    if (!SetConsoleCtrlHandler(consoleHandler, true)) {
        printf("\nERROR: Could not set control handler");
        return 1;
    }
#else
    struct sigaction sigIntHandler{};
    sigIntHandler.sa_handler = cleanup;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, nullptr);
#endif
    growtopia::database::world::world w = growtopia::database::world::get_world("start");

    cout << "Name: " << w.name << endl;
    cout << "Joinable: " << w.joinable << endl;
    cout << "Width: " << w.width << endl;
    cout << "Height: " << w.height << endl;
    cout << "Blocks Size: " << w.blocks.size() << endl;

    server.run(1024);
    return 0;
}