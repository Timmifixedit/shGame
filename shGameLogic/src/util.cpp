//
// Created by tim on 13.06.20.
//

#include "util.h"
#include <random>
namespace sh::util {
    int rng(int min, int max) {
        static std::random_device rd;
        static std::default_random_engine rEngine(rd());
        std::uniform_int_distribution dist(min, max);
        return dist(rEngine);
    }
}
