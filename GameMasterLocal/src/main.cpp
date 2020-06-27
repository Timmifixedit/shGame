//
// Created by tim on 19.06.20.
//

#include <iostream>
#include <optional>
#include <SecretHitlerGameLogic/shGameLogic.h>
#include <SecretHitlerGameLogic/enumsToString.h>
#include <printf.hpp>

#include "util.h"
#include "gameStateHandlers.h"
#include "messages.h"

int main() {
    unsigned int roundCounter = 0;
    std::optional<sh::Game> game;
    while (!(game = gameHandling::setupGame(std::cin, std::cout)).has_value());
    game->subscribe(gameHandling::createEventHandler(std::cin, std::cout, *game));
    std::cout << messages::WELCOME << std::endl;
    for (const auto &player : game->getPlayers()) {
        std::cout << player.name << " (" << sh::util::strings::toString(player.type, true) << ")" << std::endl;
    }

    while (true) {
        if (gameHandling::specialElection) {
            gameHandling::specialElection = false;
        }  else {
            game->setNextPresident();
        }

        fmt::printf(std::cout, messages::NEW_ROUND, ++roundCounter);
        std::cout << std::endl;

        gameHandling::ElectionResult electionResult;
        std::cout << messages::ELECTION_PHASE << std::endl;
        gmUtil::printGameStatus(std::cout, *game);
        while ((electionResult = gameHandling::chancellorElection(std::cin, std::cout, *game)) !=
               gameHandling::ElectionResult::SUCCESS) {
            if (electionResult == gameHandling::ElectionResult::FAILED) {
                game->setNextPresident();
                fmt::printf(std::cout, messages::NEW_ROUND, ++roundCounter);
                std::cout << std::endl;
            }

            std::cout << messages::ELECTION_PHASE << std::endl;
            gmUtil::printGameStatus(std::cout, *game);
        }

        std::cout << messages::EXECUTIVE_PHASE << std::endl;
        gameHandling::legislativePeriod(std::cin, std::cout, *game);
        std::cout << messages::ROUND_OVER << std::endl;
    }
    return 0;
}

