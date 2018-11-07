#ifndef GROWTOPIASERVER_BLOCK_TYPES_H
#define GROWTOPIASERVER_BLOCK_TYPES_H

namespace growtopia::enums {
    enum block_types : unsigned long long {
        none         = 0x00000000,
        water        = 0x04000000,
        glue         = 0x08000000,
        fire         = 0x10000000,
        color_red    = 0x20000000,
        color_green  = 0x40000000,
        color_blue   = 0x80000000,
    };
}

#endif //GROWTOPIASERVER_BLOCK_TYPES_H
