//
// Created by tim on 19.06.20.
//

#include <iostream>
#include <optional>
#include <SecretHitlerGameLogic/shGameLogic.h>
#include <SecretHitlerGameLogic/enumsToString.h>

#include "util.h"
#include "gameStateHandlers.h"

int main() {
    std::optional<sh::Game> game;
    while (!(game = gameHandling::setupGame(std::cin, std::cout)).has_value());
    game->subscribe(gameHandling::createEventHandler(std::cin, std::cout, *game));
    for (const auto &player : game->getPlayers()) {
        std::cout << player.name << " (" << sh::util::strings::toString(player.type, true) << ")" << std::endl;
    }

    while (true) {
        if (gameHandling::specialElection) {
            gameHandling::specialElection = false;
        }  else {
            game->setNextPresident();
        }

        gameHandling::ElectionResult electionResult;
        gmUtil::printGameStatus(std::cout, *game);
        while ((electionResult = gameHandling::chancellorElection(std::cin, std::cout, *game)) !=
               gameHandling::ElectionResult::SUCCESS) {
            if (electionResult == gameHandling::ElectionResult::FAILED) {
                game->setNextPresident();
                gmUtil::printGameStatus(std::cout, *game);
            }

            gmUtil::printGameStatus(std::cout, *game);
        }

        gameHandling::legislativePeriod(std::cin, std::cout, *game);
    }
    return 0;
}

