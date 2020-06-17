//
// Created by tim on 16.06.20.
//

#include <gtest/gtest.h>

#include "shGameLogic.h"

TEST(player_test, equal) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    EXPECT_EQ(p1, p2);
}

TEST(player_test, unequal) {
    using namespace sh;
    Player p1("2", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    EXPECT_NE(p1, p2);
}

TEST(player_test, unequal1) {
    using namespace sh;
    Player p1("1", Player::Type::Fascist, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    EXPECT_NE(p1, p2);
}

TEST(player_test, unequal2) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    EXPECT_NE(p1, p2);
}
