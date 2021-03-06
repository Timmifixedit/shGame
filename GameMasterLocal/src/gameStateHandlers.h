//
// Created by tim on 19.06.20.
//

#ifndef SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H
#define SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H

#include <SecretHitlerGameLogic/shGameLogic.h>
#include <optional>
#include <iostream>

namespace gameHandling {
    enum class ElectionResult {
        SUCCESS, FAILED, INPUT_FAILURE
    };

    /**
     * Prompts the user for inputs to set up a game. Errors are reported to the user.
     * @param in input stream to read the user data from
     * @param out output stream to report to the user
     * @return game instance or nothing if errors occurred.
     */
    auto setupGame(std::istream &input, std::ostream &output) -> std::optional<sh::Game>;

    ElectionResult chancellorElection(std::istream &in, std::ostream &out, sh::Game &game);

    void legislativePeriod(std::istream &in, std::ostream &out, sh::Game &game);

    sh::GameEventHandler createEventHandler(std::istream &in, std::ostream &out, sh::Game &game);

    extern bool specialElection;
    extern bool veto;
}

#endif //SECRETHITLERGAMEMASTER_GAMESTATEHANDLERS_H
