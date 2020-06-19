//
// Created by tim on 19.06.20.
//

#include <iostream>
#include <optional>
#include <SecretHitlerGameLogic/shGameLogic.h>

#include "util.h"
#include "gameStateHandlers.h"

int main() {
    std::optional<sh::Game> game;
    while (!(game = gameHandling::setupGame(std::cin, std::cout)).has_value());

    game->setNextPresident();
    auto pres = game->getPlayerByCurrentRole(sh::Player::GovernmentRole::President);
    std::cout << (*pres)->name << std::endl;
    while (!gameHandling::chancellorElection(std::cin, std::cout, *game));
    std::cout << "Success" << std::endl;
    return 0;
}

