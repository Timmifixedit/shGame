//
// Created by tim on 13.06.20.
//

#include <gtest/gtest.h>
#include <vector>
#include "shGameLogic.h"
#include <string>
#include <optional>


TEST(game_logic_game_test, ctor_players) {
    std::vector<std::string> names = {"A", "B", "C", "D", "E", "F", "G"};
    const sh::Game game(names);
    EXPECT_EQ(game.getPolicies().find(sh::CardType::Fascist)->second, 0);
    EXPECT_EQ(game.getPolicies().find(sh::CardType::Liberal)->second, 0);
    int fascist = 0;
    int liberal = 0;
    int hitler = 0;
    int president = 0;
    for(const auto &player : game.getPlayers()) {
        EXPECT_FALSE(player.isDead());
        if(player.role == sh::Player::GovernmentRole::President) {
            president++;
        } else if (player.role == sh::Player::GovernmentRole::Chancellor) {
            FAIL();
        }

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
    EXPECT_EQ(president, 1);
}

TEST(game_logic_game_test, ctor_invalid_number_of_players) {
    EXPECT_THROW(sh::Game game({}), std::runtime_error);
}

TEST(game_logic_game_test, ctor_invalid_number_of_players_1) {
    EXPECT_THROW(sh::Game game(std::vector<std::string>(15, "")), std::runtime_error);
}

TEST(game_logic_test, game_get_player_by_name) {
    sh::Game game({"A", "B", "C", "D", "E", "F", "G"});
    const auto cgame = game;
    EXPECT_EQ((*game.getPlayerByName("A"))->name, "A");
    EXPECT_EQ((*cgame.getPlayerByName("B"))->name, "B");
    EXPECT_EQ(cgame.getPlayerByName("Z"), std::nullopt);
}
