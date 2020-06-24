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
    gameHandling::ElectionResult electionResult;
    gmUtil::printGameStatus(std::cout, *game);
    while ((electionResult = gameHandling::chancellorElection(std::cin, std::cout, *game)) !=
        gameHandling::ElectionResult::SUCCESS) {
        if (electionResult == gameHandling::ElectionResult::FAILED) {
            game->setNextPresident();
        }

        gmUtil::printGameStatus(std::cout, *game);
    }
    while (!gameHandling::legislativePeriod(std::cin, std::cout, *game));
    std::cout << "Success" << std::endl;
    return 0;
}

