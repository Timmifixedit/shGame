//
// Created by tim on 19.06.20.
//

#ifndef SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H
#define SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H

#include <SecretHitlerGameLogic/shGameLogic.h>
#include <optional>
#include <iostream>

namespace gameHandling {
    /**
     * Prompts the user for inputs to set up a game. Errors are reported to the user.
     * @param in input stream to read the user data from
     * @param out output stream to report to the user
     * @return game instance or nothing if errors occurred.
     */
    auto setupGame(std::istream &input, std::ostream &output) -> std::optional<sh::Game>;

    bool chancellorElection(std::istream &in, std::ostream &out, sh::Game &game);

    bool legislativePeriod(std::istream &in, std::ostream &out, sh::Game &game);
}

#endif //SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H
