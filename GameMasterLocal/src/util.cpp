//
// Created by tim on 19.06.20.
//

#include <iostream>
#include <sstream>
#include <SecretHitlerGameLogic/enumsToString.h>

#include "util.h"
#include "printf.hpp"
#include "messages.h"

namespace gmUtil {
    std::string promptForInput(const std::string &msg, std::istream &in, std::ostream &out) {
        out << msg << std::endl;
        std::string input;
        std::getline(in, input);
        return input;
    }

    auto splitString(const std::string &string, char delimiter) -> std::vector<std::string> {
        std::vector<std::string> ret;
        std::stringstream tmp(string);
        std::string part;
        while (std::getline(tmp, part, delimiter)) {
            ret.emplace_back(part);
        }

        return ret;
    }

    auto parseRuleType(const std::string &string) -> std::optional<sh::RuleSetType> {
        constexpr auto STANDARD_RULES = "standard";
        if (string == STANDARD_RULES) {
            return sh::RuleSetType::Standard;
        }

        return {};
    }

    bool getConfirmation(const std::string &msg, std::istream &in, std::ostream &out) {
        constexpr auto CONFIRM = "yes";
        constexpr auto CONFIRM_SUCCESS = "Confirmation successful";
        constexpr auto CONFIRM_ABORT = "Aborted";
        if (promptForInput(msg, in, out) == CONFIRM) {
            out << CONFIRM_SUCCESS << std::endl;
            return true;
        } else {
            out << CONFIRM_ABORT << std::endl;
            return false;
        }
    }

    void printGameStatus(std::ostream &out, const sh::Game &game) {
        using namespace sh::util::strings;
        out << messages::PLAYER_ORDER << std::endl;
        for (const auto &player : game.getPlayers()) {
            out << player.name << " ";
            if (player.isInGovernment() && player.role.has_value()) {
                fmt::printf(out, messages::PLAYER_IN_GOV, toString(*player.role));
            } else if (player.role.has_value()) {
                fmt::printf(out, messages::PLAYER_IS_CANDIDATE, toString(*player.role));
            } else if (player.isInGovernment()) {
                fmt::printf(out, messages::PLAYER_IN_LAST_GOV);
            }

            out << std::endl;
        }
    }
}
