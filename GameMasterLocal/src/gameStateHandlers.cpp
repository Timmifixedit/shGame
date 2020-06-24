//
// Created by tim on 19.06.20.
//
#include <vector>
#include <string>
#include <unordered_set>
#include <SecretHitlerGameLogic/rules.h>
#include <SecretHitlerGameLogic/enumsToString.h>
#include <sstream>
#include <printf.hpp>

#include "gameStateHandlers.h"
#include "messages.h"
#include "util.h"

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

    ElectionResult chancellorElection(std::istream &in, std::ostream &out, sh::Game &game) {
        std::string chancellorName = gmUtil::promptForInput(messages::ELECT_CHANCELLOR, in, out);
        const char *message = chancellorName.empty() ? messages::CONFIRM_ELECTION_FAILED : messages::CONFIRM_DECISION;
        bool confirm = gmUtil::getConfirmation(message, in, out);
        if (chancellorName.empty() && confirm) {
            return ElectionResult::FAILED;
        } else if (!confirm) {
            return ElectionResult::INPUT_FAILURE;
        }

        std::optional<sh::SetRoleStatus> status = game.setPlayerRole(chancellorName,
                sh::Player::GovernmentRole::Chancellor);
        if (!status.has_value()) {
            out << messages::PLAYER_NOT_FOUND << std::endl;
            return ElectionResult::INPUT_FAILURE;
        } else if (status == sh::SetRoleStatus::Ineligible) {
            fmt::printf(out, messages::PLAYER_INELIGIBLE, chancellorName);
            out << std::endl;
            return ElectionResult::INPUT_FAILURE;
        } else if (status == sh::SetRoleStatus::PlayerIsDead) {
            out << messages::PLAYER_DEAD << std::endl;
            return ElectionResult::INPUT_FAILURE;
        } else {
            game.electGovernment();
            out << messages::ELECTION_SUCCESS << std::endl;
            return ElectionResult::SUCCESS;
        }
    }

    bool legislativePeriod(std::istream &in, std::ostream &out, sh::Game &game) {
        constexpr unsigned int N_DRAW_CARDS = 3;
        auto currentPres = game.getPlayerByCurrentRole(sh::Player::GovernmentRole::President);
        if (!currentPres.has_value()) {
            throw std::runtime_error("No president in current government");
        }

        sh::CardRange cards = game.drawCards(N_DRAW_CARDS);
        auto promptPlayerForCard = [&in, &out, &cards]() {
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
                }
            } while (invalidInput || !gmUtil::getConfirmation(messages::CONFIRM_DECISION, in , out));
            return *choice;
        };

        bool validCard = true;
        do {
            validCard = true;
            fmt::printf(out, messages::PRES_DISCARD_CARD, (*currentPres)->name);
            out << std::endl;
            sh::CardType choice = promptPlayerForCard();
            if (!cards.discard(choice)) {
                fmt::printf(out, messages::INVALID_POLICY, sh::util::strings::toString(choice));
                out << std::endl;
                validCard = false;
            }
        } while (!validCard);

        auto currentChancellor = game.getPlayerByCurrentRole(sh::Player::GovernmentRole::Chancellor);
        if (!currentChancellor.has_value()) {
            throw std::runtime_error("No chancellor in current government");
        }

        do {
            validCard = true;
            fmt::printf(out, messages::PLAY_POLICY, (*currentChancellor)->name);
            out << std::endl;
            sh::CardType choice = promptPlayerForCard();
            if (!cards.selectForPolicy(choice)) {
                fmt::printf(out, messages::INVALID_POLICY, sh::util::strings::toString(choice));
                out << std::endl;
                validCard = false;
            }
        } while (!validCard);

        if (!cards.applyToGame()) {
            throw std::runtime_error("Failed to play policy card");
        }

        return true;
    }
}
