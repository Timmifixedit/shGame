//
// Created by tim on 13.06.20.
//

#include <gtest/gtest.h>
#include <vector>
#include "shGameLogic.h"
#include <string>


TEST(game_logic_game_test, ctor) {
    std::vector<std::string> names = {"A", "B", "C", "D", "E", "F", "G"};
    const sh::Game game(names);
    int fascist = 0;
    int liberal = 0;
    int hitler = 0;
    for(const auto &player : game.getPlayers()) {
        EXPECT_FALSE(player.isDead());
        switch (player.type) {
            case sh::Player::Type::Fascist:
                fascist++;
                break;
            case sh::Player::Type::Liberal:
                liberal++;
                break;
            case sh::Player::Type::Hitler:
                hitler++;
                break;
        }
    }

    unsigned int expectedLib = sh::Game::numberOfLiberals(names.size());
    unsigned int expectedFasc = names.size() - expectedLib - 1;

    EXPECT_EQ(fascist, expectedFasc);
    EXPECT_EQ(liberal, expectedLib);
    EXPECT_EQ(hitler, 1);
}