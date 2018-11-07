#ifndef GROWTOPIASERVER_SYSTEM_H
#define GROWTOPIASERVER_SYSTEM_H

#include <string>
#ifdef WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

namespace growtopia::helpers {
    void mkdir(std::string path);
}


#endif //GROWTOPIASERVER_SYSTEM_H
