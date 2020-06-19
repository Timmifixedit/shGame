//
// Created by tim on 19.06.20.
//
#include <vector>
#include <string>
#include <SecretHitlerGameLogic/rules.h>

#include "gameStateHandlers.h"
#include "messages.h"
#include "util.h"

namespace gameHandling{
    auto setupGame(std::istream &in, std::ostream &out) -> std::optional<sh::Game> {
        std::string playersIn = gmUtil::promptForInput(INPUT_PLAYER_NAMES, in, out);
        std::vector<std::string> playerNames = gmUtil::splitString(playersIn, ' ');
        for (const auto &playerName : playerNames) {
            if (playerName.empty()) {
                out << EMPTY_NAME << std::endl;
                return {};
            }
        }
        std::string ruleSetIn = gmUtil::promptForInput(INPUT_RULE_SET, in, out);
        std::optional<sh::RuleSetType> ruleType = gmUtil::parseRuleType(ruleSetIn);
        if (!ruleType.has_value()) {
            out << INVALID_RULE_SET << std::endl;
            return {};
        }

        try {
            return sh::Game(playerNames, sh::createRuleSet(*ruleType));
        } catch (const std::runtime_error &e) {
            out << e.what() << std::endl;
            return {};
        }
    }
}
