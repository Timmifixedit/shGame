//
// Created by tim on 13.06.20.
//

#include <gtest/gtest.h>
#include "util.h"

TEST(util_test, rng_bounds_pos) {
    int rngVal = util::rng(0, 10);
    EXPECT_LE(rngVal, 10);
    EXPECT_GE(rngVal, 0);
}

TEST(util_test, rng_bounds_neg) {
    int rngVal = util::rng(-17, 3);
    EXPECT_LE(rngVal, 3);
    EXPECT_GE(rngVal, -17);
}

TEST(uti_test, select_random_array) {
    int array[] = {1, 2, 3, 4, 5, 6};
    auto *elem = util::selectRandom(array);
    *elem = 17;
    bool found = false;
    for(const auto &i : array) {
        if(i == 17) {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
}

TEST(uti_test, select_random_container) {
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    auto elem = util::selectRandom(vec);
    *elem = 17;
    bool found = false;
    for(const auto &i : vec) {
        if(i == 17) {
            found = true;
            break;
        }
    }

    EXPECT_TRUE(found);
}

