//
// Created by tim on 15.06.20.
//

#include "GameEvent.h"
namespace sh {
    GameEvent::GameEvent(GameEvent::Type type) : type(type) {}

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

    LiberalsWin::LiberalsWin() : GameEvent(Type::LiberalsWin) {}

    bool LiberalsWin::condition(const Game &gameState, Trigger) const {
        constexpr unsigned int NUM_LIB_CARDS_WIN = 5;
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

    FascistsWin::FascistsWin() : GameEvent(Type::FascistsWin) {}

    bool FascistsWin::condition(const Game &gameState, Trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_WIN = 6;
        constexpr unsigned int NUM_HITLER_ELECTION_THRESH = 4;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        bool hitlerChancellor = false;
        for (const auto &player : gameState.getPlayers()) {
            if (player.type == Player::Type::Hitler && player.role == Player::GovernmentRole::Chancellor) {
                hitlerChancellor = true;
                break;
            }
        }

        return (hitlerChancellor && fasCards >= NUM_HITLER_ELECTION_THRESH) || fasCards >= NUM_FAS_CARDS_WIN;
    }

    InvestigateLoyalty::InvestigateLoyalty() : GameEvent(Type::InvestigateLoyalty) {}

    bool InvestigateLoyalty::condition(const Game &gameState, Trigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_LOYAL = 2;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == Trigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_LOYAL;
    }

    SpecialElection::SpecialElection() : GameEvent(Type::SpecialElection) {}

    bool SpecialElection::condition(const Game &gameState, Trigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_ELECTION = 3;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == Trigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_ELECTION;
    }

    Execution::Execution() : GameEvent(Type::Execution) {}

    bool Execution::condition(const Game &gameState, Trigger trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_EXEC = 4;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return trigger == Trigger::FascistPolicyPlayed && fasCards == NUM_FAS_CARDS_EXEC;
    }

    Veto::Veto() : GameEvent(Type::Veto) {}

    bool Veto::condition(const Game &gameState, Trigger) const {
        constexpr unsigned int NUM_FAS_CARDS_VETO = 5;
        unsigned int fasCards = this->getPolicyNumbers(gameState).second;
        return fasCards >= NUM_FAS_CARDS_VETO;
    }
}
