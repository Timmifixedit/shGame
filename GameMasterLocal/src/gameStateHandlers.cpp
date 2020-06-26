//
// Created by tim on 19.06.20.
//
#include <vector>
#include <string>
#include <unordered_set>
#include <SecretHitlerGameLogic/shGameLogic.h>
#include <SecretHitlerGameLogic/rules.h>
#include <SecretHitlerGameLogic/enumsToString.h>
#include <sstream>
#include <printf.hpp>

#include "gameStateHandlers.h"
#include "messages.h"
#include "util.h"

namespace gameHandling{
    bool specialElection = false;
    bool veto = false;

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
            game.advanceElectionTracker();
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

    void legislativePeriod(std::istream &in, std::ostream &out, sh::Game &game) {
        constexpr unsigned int N_DRAW_CARDS = 3;
        auto currentPres = game.getPlayerByCurrentRole(sh::Player::GovernmentRole::President);
        if (!currentPres.has_value()) {
            throw std::runtime_error("No president in current government");
        }

        fmt::printf(out, messages::PRES_DISCARD_CARD, (*currentPres)->name);
        out << std::endl;
        sh::CardRange cards = game.drawCards(N_DRAW_CARDS);
        std::optional<sh::CardType> choice = gmUtil::promptPlayerForCard(in, out, cards);
        if (!choice.has_value()) {
            throw std::runtime_error("President cannot use veto!");
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
        if (veto) {
            out << messages::VETO_HINT << std::endl;
        }

        choice = gmUtil::promptPlayerForCard(in, out, cards, veto);
        if (!choice.has_value() && veto) {
            out << messages::VETO_USED << std::endl;
            game.advanceElectionTracker();
            return;
        } else if (choice.has_value() && (!cards.selectForPolicy(*choice) || !cards.applyToGame())) {
            throw std::runtime_error("Failed to play policy card");
        }
    }

    sh::GameEventHandler createEventHandler(std::istream &in, std::ostream &out, sh::Game &game) {
        using namespace messages;
        using CType = sh::CardType;
        using GovRole = sh::Player::GovernmentRole;
        using PType = sh::Player::Type;
        return [&in, &out, &game](sh::GameEventType type) {
            out << EVENT_PHASE << std::endl;
            switch (type) {
                case sh::GameEventType::LiberalsWin: {
                    const sh::Game &localGame = game;
                    if (localGame.getHitler()->isDead()) {
                        out << LIBERALS_WON_KILL << std::endl;
                        return;
                    }

                    fmt::printf(out, LIBERALS_WON_CARDS, localGame.getPolicies().find(CType::Liberal)->second);
                    out << std::endl;
                    std::exit(0);
                }
                case sh::GameEventType::FascistsWin: {
                    const sh::Game &localGame = game;
                    const sh::Player &hitler = *localGame.getHitler();
                    if (hitler.isInGovernment() && hitler.role == GovRole ::Chancellor) {
                        out << FASCISTS_WON_HITLER << std::endl;
                        break;
                    }

                    fmt::printf(out, FASCISTS_WON_CARDS, localGame.getPolicies().find(CType::Fascist)->second);
                    out << std::endl;
                    std::exit(0);
                }
                case sh::GameEventType::InvestigateLoyalty: {
                    const sh::Game &localGame = game;
                    auto pres = localGame.getPlayerByCurrentRole(GovRole::President);
                    if (!pres.has_value()) {
                        throw std::runtime_error("No player is president");
                    }

                    fmt::printf(out, PRES_INVESTIGATE, (*pres)->name);
                    out << std::endl;
                    std::string pName = gmUtil::promptForPlayerAndConfirm(in, out, localGame);
                    PType playerType = (*localGame.getPlayerByName(pName))->type;
                    playerType = playerType == PType::Liberal ? playerType : PType::Fascist;
                    out << pName << " is " << sh::util::strings::toString(playerType) << std::endl;
                    break;
                }
                case sh::GameEventType::Execution: {
                    auto pres = game.getPlayerByCurrentRole(GovRole::President);
                    if (!pres.has_value()) {
                        throw std::runtime_error("No player is president");
                    }

                    fmt::printf(out, PRES_EXECUTION, (*pres)->name);
                    out << std::endl;
                    std::string pName = gmUtil::promptForPlayerAndConfirm(in, out, game);
                    game.killPlayer(pName);
                    fmt::printf(out, PLAYER_EXECUTED, pName);
                    out << std::endl;
                    break;
                }
                case sh::GameEventType::SpecialElection: {
                    auto pres = game.getPlayerByCurrentRole(GovRole::President);
                    if (!pres.has_value()) {
                        throw std::runtime_error("No player is president");
                    }

                    fmt::printf(out, PRES_SPECIAL_ELECTION, (*pres)->name);
                    out << std::endl;
                    std::optional<sh::SetRoleStatus> status{};
                    gmUtil::printGameStatus(out, game);
                    while ((status = game.setNextPresident(gmUtil::promptForPlayerAndConfirm(in, out, game)))
                        != sh::SetRoleStatus::Success) {
                        if (!status.has_value()) {
                            throw std::runtime_error("Player name not found. Cannot perform special election!");
                        }

                        switch (*status) {
                            case sh::SetRoleStatus::PlayerIsDead:
                                out << PLAYER_DEAD << std::endl;
                                break;
                            case sh::SetRoleStatus::Ineligible:
                                out << PLAYER_INELIGIBLE << std::endl;
                                break;
                            default:
                                throw std::runtime_error("That's not how booleans are supposed to work!");
                        }
                    }

                    specialElection = true;
                    break;
                }
                case sh::GameEventType::Veto:
                    veto = true;
                    break;
                case sh::GameEventType::RandomPolicy: {
                    sh::CardType cardPlayed = game.playRandomPolicy();
                    fmt::printf(out, RANDOM_POLICY, sh::util::strings::toString(cardPlayed));
                    out << std::endl;
                    break;
                }
                default:
                    throw std::runtime_error("Event type not supported yet!");
            }
        };
    }
}
