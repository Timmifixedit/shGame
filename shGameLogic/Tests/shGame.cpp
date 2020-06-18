//
// Created by tim on 13.06.20.
//

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <optional>

#include "shGameLogic.h"

//--------------- ctor -------------------------------------------------------------------------------------------------

TEST(game_logic_game_test, ctor_players) {
    std::vector<std::string> names = {"A", "B", "C", "D", "E", "F", "G"};
    sh::Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_EQ(game.getPolicies().find(sh::CardType::Fascist)->second, 0);
    EXPECT_EQ(game.getPolicies().find(sh::CardType::Liberal)->second, 0);
    int fascist = 0;
    int liberal = 0;
    int hitler = 0;
    for(const auto &player : game.getPlayers()) {
        EXPECT_FALSE(player.isDead());
        EXPECT_FALSE(player.role.has_value());

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

TEST(game_logic_game_test, ctor_invalid_number_of_players) {
    EXPECT_THROW(sh::Game game({}, {}), std::runtime_error);
}

TEST(game_logic_game_test, ctor_invalid_number_of_players_1) {
    EXPECT_THROW(sh::Game game(std::vector<std::string>(15, ""), {}), std::runtime_error);
}

//--------------- get by name ------------------------------------------------------------------------------------------
TEST(game_logic_test, game_get_player_by_name) {
    sh::Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const auto cgame = game;
    EXPECT_EQ((*game.getPlayerByName("A"))->name, "A");
    EXPECT_EQ((*cgame.getPlayerByName("B"))->name, "B");
    EXPECT_EQ(cgame.getPlayerByName("Z"), std::nullopt);
}

//--------------- get set roles ----------------------------------------------------------------------------------------
TEST(game_logic_test, get_set_president) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const Player &newPres = game.getPlayers().front();

    EXPECT_EQ(game.setPlayerRole(newPres.name, Role::President), SetRoleStatus::Success);
    EXPECT_EQ(newPres, **game.getPlayerByCurrentRole(Role::President));
}

TEST(game_logic_test, set_role_dead) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_TRUE(game.killPlayer("A"));
    EXPECT_EQ(game.setPlayerRole("A", Role::Chancellor), SetRoleStatus::PlayerIsDead);
    EXPECT_FALSE((*game.getPlayerByName("A"))->role.has_value());
}

TEST(game_logic_test, set_same_president) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const Player &pres = game.getPlayers().front();
    EXPECT_EQ(game.setPlayerRole(pres.name, Role::President), SetRoleStatus::Success);
    EXPECT_EQ(game.setPlayerRole(pres.name, Role::President), SetRoleStatus::Ineligible);
}

TEST(game_logic_test, set_role_player_not_found) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_FALSE(game.setPlayerRole("Z", Role::Chancellor).has_value());
}

TEST(game_logic_test, set_new_president) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const Player &pres = game.getPlayers().front();
    EXPECT_EQ(game.setPlayerRole(pres.name, Role::President), SetRoleStatus::Success);
    const Player &newPres = game.getPlayers().back();
    EXPECT_EQ(game.setPlayerRole(newPres.name, Role::President), SetRoleStatus::Success);
    EXPECT_EQ(newPres.role, Role::President);
    EXPECT_FALSE(pres.role.has_value());
}

TEST(game_logic_test, get_set_chancellor) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const Player &chancellor = game.getPlayers().front();
    EXPECT_EQ(game.setPlayerRole(chancellor.name, Role::Chancellor), SetRoleStatus::Success);
    EXPECT_EQ(chancellor.role, Role::Chancellor);
}

TEST(game_logic_test, set_chancellor_ineligible) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_EQ(game.setPlayerRole("A", Role::President), SetRoleStatus::Success);
    EXPECT_EQ(game.setPlayerRole("B", Role::Chancellor), SetRoleStatus::Success);
    (*game.getPlayerByCurrentRole(Role::Chancellor))->elect();
    EXPECT_EQ(game.setPlayerRole("A", Role::Chancellor), SetRoleStatus::Ineligible);
    EXPECT_EQ((*game.getPlayerByName("A"))->role, Role::President);
    EXPECT_EQ(game.setPlayerRole("B", Role::Chancellor), SetRoleStatus::Ineligible);
}

TEST(game_logic_test, set_new_chanellor) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    const Player &chancellor = game.getPlayers().front();
    EXPECT_EQ(game.setPlayerRole(chancellor.name, Role::Chancellor), SetRoleStatus::Success);
    const Player &newChancellor = game.getPlayers().back();
    EXPECT_EQ(game.setPlayerRole(newChancellor.name, Role::Chancellor), SetRoleStatus::Success);
    EXPECT_EQ(newChancellor.role, Role::Chancellor);
    EXPECT_FALSE(chancellor.role.has_value());
}

//--------------- get set roles ----------------------------------------------------------------------------------------
TEST(game_logic_test, set_next_president) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_EQ(game.setPlayerRole("C", Role::President), SetRoleStatus::Success);
    game.setNextPresident();
    EXPECT_FALSE((*game.getPlayerByName("C"))->role.has_value());
    EXPECT_EQ((*game.getPlayerByName("D"))->role, Role::President);
}

TEST(game_logic_test, set_next_pres_skip_dead) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_EQ(game.setPlayerRole("F", Role::President), SetRoleStatus::Success);
    EXPECT_TRUE(game.killPlayer("G"));
    game.setNextPresident();
    EXPECT_FALSE((*game.getPlayerByName("C"))->role.has_value());
    EXPECT_EQ((*game.getPlayerByName("A"))->role, Role::President);
}

TEST(game_logic_test, set_next_pres_all_dead) {
    using namespace sh;
    using Role = Player::GovernmentRole;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    auto player = *game.getPlayerByName("A");
    EXPECT_EQ(game.setPlayerRole(player->name, Role::President), SetRoleStatus::Success);
    ++player;
    while (player != game.getPlayers().end()) {
        player->kill();
        ++player;
    }

    EXPECT_THROW(game.setNextPresident(), std::runtime_error);
}

//--------------- get set roles ----------------------------------------------------------------------------------------
TEST(game_logic_test, kill_player) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, {});
    EXPECT_TRUE(game.killPlayer("A"));
    EXPECT_TRUE((*game.getPlayerByName("A"))->isDead());
}
