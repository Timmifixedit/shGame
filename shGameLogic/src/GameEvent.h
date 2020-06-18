//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_POLICYEVENT_H
#define SHGAME_POLICYEVENT_H

#include <utility>

#include "GlobalTypes.h"
#include "shGameLogic.h"

namespace sh {

    class GameEvent {
    public:
        enum class Type {
            LiberalsWin, FascistsWin,
            InvestigateLoyalty, Execution,
            PolicyPeak, SpecialElection, Veto
        };

        enum class Trigger {
            FascistPolicyPlayed, LiberalPolicyPlayed, PlayerExecuted
        };

        /**
         * Returns if the condition for the game event is met
         * @param numFascistCards number of fascist cards currently on the policy board
         * @param numLiberalCards number of liberal cards currently on the policy board
         * @param currentPolicy the policy to be played in this round
         * @return true if event condition is met, false otherwise
         */
        [[nodiscard]] virtual bool condition(const Game &gameState, Trigger trigger) const = 0;

        const Type type;
    protected:
        [[nodiscard]] static auto getPolicyNumbers(const Game &gameState) -> std::pair<unsigned int, unsigned int>;
        explicit GameEvent(Type type);
    };

    class LiberalsWin : public GameEvent {
    public:
        LiberalsWin();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };

    class FascistsWin : public GameEvent {
    public:
        FascistsWin();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };

    class InvestigateLoyalty : public GameEvent {
    public:
        InvestigateLoyalty();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };

    class SpecialElection : public GameEvent {
    public:
        SpecialElection();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };

    class Execution : public GameEvent {
    public:
        Execution();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };

    class Veto : public GameEvent {
    public:
        Veto();
        [[nodiscard]] bool condition(const Game &gameState, Trigger trigger) const override;
    };
}

#endif //SHGAME_POLICYEVENT_H
