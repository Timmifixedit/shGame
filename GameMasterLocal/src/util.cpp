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
        out << messages::POLICY_BOARD << std::endl;
        for (auto [type, num] : game.getPolicies()) {
            out << toString(type) << ": " << num << std::endl;
        }

        out << messages::PLAYER_ORDER << std::endl;
        for (const auto &player : game.getPlayers()) {
            out << player.name << " ";
            if (player.isDead()) {
                out << "(dead)";
            } if (player.isInGovernment() && player.role.has_value()) {
                fmt::printf(out, messages::PLAYER_IN_GOV, toString(*player.role));
            } else if (player.role.has_value()) {
                fmt::printf(out, messages::PLAYER_IS_CANDIDATE, toString(*player.role));
            } else if (player.isInGovernment()) {
                fmt::printf(out, messages::PLAYER_IN_LAST_GOV);
            }

            out << std::endl;
        }
    }

    std::string promptForPlayer(std::istream &in, std::ostream &out, const sh::Game &game) {
        using namespace messages;
        std::string playerName;
        while (!game.getPlayerByName(playerName = gmUtil::promptForInput(CHOOSE_PLAYER,
                in, out)).has_value()) {
            out << PLAYER_NOT_FOUND << std::endl;
        }

        return playerName;
    }

    std::string promptForPlayerAndConfirm(std::istream &in, std::ostream &out, const sh::Game &game) {
        std::string pName;
        do {
            pName = gmUtil::promptForPlayer(in, out, game);
        } while (!gmUtil::getConfirmation(messages::CONFIRM_DECISION, in, out));
        return pName;
    }

    sh::CardType promptPlayerForCard(std::istream &in, std::ostream &out, const sh::CardRange &cards) {
        bool invalidInput = false;
        std::optional<sh::CardType> choice;
        do {
            invalidInput = false;
            for (auto card : cards) {
                out << sh::util::strings::toString(card) << " | ";
            }

            out << std::endl;
            std::string playerInput = gmUtil::promptForInput(messages::CHOOSE_CARD, in, out);
            choice = sh::util::strings::toCardType(playerInput);
            if (!choice.has_value()) {
                fmt::printf(out, messages::INVALID_CARD_TYPE, playerInput);
                out << std::endl;
                invalidInput = true;
            } else {
                if (std::find(cards.begin(), cards.end(), *choice) == cards.end()) {
                    fmt::printf(out, messages::INVALID_POLICY, sh::util::strings::toString(*choice));
                    out << std::endl;
                    invalidInput = true;
                }
            }
        } while (invalidInput || !gmUtil::getConfirmation(messages::CONFIRM_DECISION, in , out));
        return *choice;
    }
}
