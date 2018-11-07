//
// Created by -server- on 11/6/2018.
//

#ifndef GROWTOPIASERVER_RANDOM_H
#define GROWTOPIASERVER_RANDOM_H

#include <random>

namespace growtopia::helpers {
    template<typename T>
    T random_number(const T min, const T max) {
        static std::random_device rdev;
        static std::default_random_engine re(rdev());
        typedef typename std::conditional<
                std::is_floating_point<T>::value,
                std::uniform_real_distribution<T>,
                std::uniform_int_distribution<T>>::type dist_type;
        dist_type uni(min, max);
        return static_cast<T>(uni(re));
    }
}

#endif //GROWTOPIASERVER_RANDOM_H
