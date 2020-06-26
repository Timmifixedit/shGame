//
// Created by tim on 15.06.20.
//

#include "GameEvent.h"
namespace sh {
    GameEvent::GameEvent(GameEventType type) : type(type) {}

    auto GameEvent::getPolicyNumbers(const Game &gameState) -> std::pair<unsigned int, unsigned int> {
        auto getNumPolicies = [&gameState] (CardType cardType) {
            const auto policies = gameState.getPolicies().find(cardType);
            if (policies == gameState.getPolicies().end()) {
                throw std::runtime_error("Policy board is corrupt!");
            }

            return policies->second;
        };

        return {getNumPolicies(CardType::Liberal), getNumPolicies(CardType::Fascist)};
    }

    LiberalsWin::LiberalsWin() : GameEvent(GameEventType::LiberalsWin) {}

    bool LiberalsWin::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int NUM_LIB_CARDS_WIN = 5;
        if (trigger != GameEventTrigger::LiberalPolicyPlayed && trigger != GameEventTrigger::PlayerExecuted) {
            return false;
        }

        unsigned int libCards = this->getPolicyNumbers(gameState).first;
        bool hitlerDead = false;
        for (const auto &player : gameState.getPlayers()) {
            if (player.type == Player::Type::Hitler && player.isDead()) {
                hitlerDead = true;
                break;
            }
        }

        return hitlerDead || libCards >= NUM_LIB_CARDS_WIN;
    }

    FascistsWin::FascistsWin() : GameEvent(GameEventType::FascistsWin) {}

    bool FascistsWin::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_WIN = 6;
        constexpr unsigned int NUM_HITLER_ELECTION_THRESH = 3;
        if (trigger != GameEventTrigger::FascistPolicyPlayed && trigger != GameEventTrigger::PlayerElected) {
            return false;
        }

        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        bool hitlerChancellor = false;
        for (const auto &player : gameState.getPlayers()) {
            if (player.type == Player::Type::Hitler && player.role == Player::GovernmentRole::Chancellor &&
                player.isInGovernment()) {
                hitlerChancellor = true;
                break;
            }
        }

        return (hitlerChancellor && fasCards >= NUM_HITLER_ELECTION_THRESH && trigger == GameEventTrigger::PlayerElected) ||
                fasCards >= NUM_FAS_CARDS_WIN;
    }

    InvestigateLoyalty::InvestigateLoyalty() : GameEvent(GameEventType::InvestigateLoyalty) {}

    bool InvestigateLoyalty::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_LOYAL = 2;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == GameEventTrigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_LOYAL;
    }

    SpecialElection::SpecialElection() : GameEvent(GameEventType::SpecialElection) {}

    bool SpecialElection::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_ELECTION = 3;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == GameEventTrigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_ELECTION;
    }

    Execution::Execution() : GameEvent(GameEventType::Execution) {}

    bool Execution::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_EXEC = 4;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == GameEventTrigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_EXEC;
    }

    Veto::Veto() : GameEvent(GameEventType::Veto) {}

    bool Veto::condition(const Game &gameState, GameEventTrigger) const {
        constexpr unsigned int NUM_FAS_CARDS_VETO = 5;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return fasCards >= NUM_FAS_CARDS_VETO;
    }

    RandomPolicy::RandomPolicy() : GameEvent(GameEventType::RandomPolicy){}

    bool RandomPolicy::condition(const Game &gameState, GameEventTrigger trigger) const {
        constexpr unsigned int MAX_ELECTION_TRACKER = 3;
        return trigger == GameEventTrigger::ElectionTrackerAdvanced &&
            gameState.getElectionTracker() >= MAX_ELECTION_TRACKER;
    }
}
