//
// Created by tim on 19.06.20.
//
#include <vector>
#include <string>
#include <unordered_set>
#include <SecretHitlerGameLogic/rules.h>

#include "gameStateHandlers.h"
#include "messages.h"
#include "util.h"

namespace gameHandling{
    auto setupGame(std::istream &input, std::ostream &output) -> std::optional<sh::Game> {
        const std::string playersIn = gmUtil::promptForInput(INPUT_PLAYER_NAMES, input, output);
        const std::vector<std::string> playerNames = gmUtil::splitString(playersIn, ' ');
        std::unordered_set<std::string> validNames;
        validNames.reserve(playerNames.size());
        for (const auto &playerName : playerNames) {
            if(!playerName.empty() && !validNames.emplace(playerName).second) {
                output << DUPLICATE_NAME << std::endl;
                return {};
            }
        }

        const std::string ruleSetIn = gmUtil::promptForInput(INPUT_RULE_SET, input, output);
        const std::optional<sh::RuleSetType> ruleType = gmUtil::parseRuleType(ruleSetIn);
        if (!ruleType.has_value()) {
            output << INVALID_RULE_SET << std::endl;
            return {};
        }

        try {
            return sh::Game(validNames, sh::createRuleSet(*ruleType));
        } catch (const std::runtime_error &e) {
            output << e.what() << std::endl;
            return {};
        }
    }

    bool chancellorElection(std::istream &in, std::ostream &out, sh::Game &game) {
        std::string chancellorName = gmUtil::promptForInput(ELECT_CHANCELLOR, in, out);
        std::optional<sh::SetRoleStatus> status = game.setPlayerRole(chancellorName,
                sh::Player::GovernmentRole::Chancellor);
        if (!status.has_value()) {
            out << PLAYER_NOT_FOUND << std::endl;
            return false;
        } else if (status == sh::SetRoleStatus::Ineligible) {
            out << PLAYER_INELIGIBLE << std::endl;
            return false;
        } else if (status == sh::SetRoleStatus::PlayerIsDead) {
            out << PLAYER_DEAD << std::endl;
            return false;
        } else {
            game.electGovernment();
            out << ELECTION_SUCCESS << std::endl;
            return true;
        }
    }
}
