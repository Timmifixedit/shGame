//
// Created by tim on 19.06.20.
//

#include <iostream>
#include <optional>
#include <SecretHitlerGameLogic/shGameLogic.h>

#include "util.h"
#include "gameStateHandlers.h"

int main(int , char* []) {
    std::optional<sh::Game> game;
    while (!(game = gameHandling::setupGame(std::cin, std::cout)).has_value());
    for (const auto &player : game->getPlayers()) {
        std::cout << player.name << std::endl;
    }

    std::cout << "Success" << std::endl;
    return 0;
}

