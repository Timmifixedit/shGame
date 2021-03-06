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

TEST(player_test, equal1) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    p1.kill();
    p2.kill();
    EXPECT_EQ(p1, p2);
}

TEST(player_test, equal2) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    p1.elect();
    p2.elect();
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

TEST(player_test, unequal3) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    p1.elect();
    EXPECT_NE(p1, p2);
}

TEST(player_test, unequal4) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    Player p2("1", Player::Type::Liberal, Player::GovernmentRole::Chancellor);
    p1.kill();
    EXPECT_NE(p1, p2);
}

TEST(player_test, elect) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::President);
    p1.elect();
    EXPECT_TRUE(p1.isInGovernment());
}

TEST(player_test, elect_no_rolo) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal);
    EXPECT_THROW(p1.elect(), std::runtime_error);
}

TEST(player_test, remove_from_gov) {
    using namespace sh;
    Player p1("1", Player::Type::Liberal, Player::GovernmentRole::President);
    p1.elect();
    p1.removeFromGovernment();
    EXPECT_FALSE(p1.isInGovernment());
    EXPECT_FALSE(p1.role.has_value());
}
