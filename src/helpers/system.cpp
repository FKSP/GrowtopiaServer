#include "system.h"

using namespace std;
using namespace growtopia;

void helpers::makedir(string path) {
#if WIN32
    _mkdir("worlds");
#else
    mkdir("worlds",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}
