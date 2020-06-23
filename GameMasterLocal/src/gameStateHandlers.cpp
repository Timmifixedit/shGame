//
// Created by tim on 19.06.20.
//
#include <vector>
#include <string>
#include <unordered_set>
#include <SecretHitlerGameLogic/rules.h>
#include <SecretHitlerGameLogic/enumsToString.h>
#include <sstream>

#include "gameStateHandlers.h"
#include "messages.h"
#include "util.h"
#include "printf.hpp"

namespace gameHandling{
    auto setupGame(std::istream &input, std::ostream &output) -> std::optional<sh::Game> {
        const std::string playersIn = gmUtil::promptForInput(messages::INPUT_PLAYER_NAMES, input, output);
        const std::vector<std::string> playerNames = gmUtil::splitString(playersIn, ' ');
        std::unordered_set<std::string> validNames;
        validNames.reserve(playerNames.size());
        for (const auto &playerName : playerNames) {
            if(!playerName.empty() && !validNames.emplace(playerName).second) {
                output << messages::DUPLICATE_NAME << std::endl;
                return {};
            }
        }

        const std::string ruleSetIn = gmUtil::promptForInput(messages::INPUT_RULE_SET, input, output);
        const std::optional<sh::RuleSetType> ruleType = gmUtil::parseRuleType(ruleSetIn);
        if (!ruleType.has_value()) {
            output << messages::INVALID_RULE_SET << std::endl;
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
        std::string chancellorName = gmUtil::promptForInput(messages::ELECT_CHANCELLOR, in, out);
        std::optional<sh::SetRoleStatus> status = game.setPlayerRole(chancellorName,
                sh::Player::GovernmentRole::Chancellor);
        if (!status.has_value()) {
            out << messages::PLAYER_NOT_FOUND << std::endl;
            return false;
        } else if (status == sh::SetRoleStatus::Ineligible) {
            fmt::printf(out, messages::PLAYER_INELIGIBLE, chancellorName);
            out << std::endl;
            return false;
        } else if (status == sh::SetRoleStatus::PlayerIsDead) {
            out << messages::PLAYER_DEAD << std::endl;
            return false;
        } else {
            game.electGovernment();
            out << messages::ELECTION_SUCCESS << std::endl;
            return true;
        }
    }

    bool legisltivePeriod(std::istream &in, std::ostream &out, sh::Game &game) {
        constexpr unsigned int N_DRAW_CARDS = 3;
        auto currentPres = game.getPlayerByCurrentRole(sh::Player::GovernmentRole::President);
        if (!currentPres.has_value()) {
            throw std::runtime_error("No president in current government");
        }

        fmt::printf(out, messages::PRES_DISCARD_CARD, (*currentPres)->name);
        out << std::endl;
        sh::CardRange cards = game.drawCards(N_DRAW_CARDS);
        for (auto card : cards) {
            out << sh::util::strings::toString(card) << " | ";
        }

        out << std::endl;
        std::string playerInput = gmUtil::promptForInput(messages::CHOOSE_CARD, in, out);
        std::optional<sh::CardType> choice = sh::util::strings::toCardType(playerInput);
        if (!choice.has_value()) {
            fmt::printf(out, messages::INVALID_CARD_TYPE, playerInput);
            out << std::endl;
            return false;
        }

        if (!cards.discard(*choice)) {
            throw std::runtime_error("Failed to discard card");
        }

        auto currentChancellor = game.getPlayerByCurrentRole(sh::Player::GovernmentRole::Chancellor);
        if (!currentChancellor.has_value()) {
            throw std::runtime_error("No chancellor in current government");
        }

        fmt::printf(out, messages::PLAY_POLICY, (*currentChancellor)->name);
        out << std::endl;
        for (auto card : cards) {
            out << sh::util::strings::toString(card) << " | ";
        }

        out << std::endl;
        playerInput = gmUtil::promptForInput(messages::CHOOSE_CARD, in, out);
        choice = sh::util::strings::toCardType(playerInput);
        if (!choice.has_value()) {
            fmt::printf(out, messages::INVALID_CARD_TYPE, playerInput);
            out << std::endl;
            return false;
        }

        if (!cards.selectForPolicy(*choice) || !cards.applyToGame()) {
            throw std::runtime_error("Failed to play policy card");
        }

        return true;
    }
}
