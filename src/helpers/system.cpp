#include "system.h"

using namespace std;
using namespace growtopia;

void helpers::mkdir(string path) {
#if WIN32
    _mkdir("worlds");
#else
    mkdir("worlds");
#endif
}
