//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_POLICYEVENT_H
#define SHGAME_POLICYEVENT_H

#include <utility>
#include <memory>
#include <functional>
#include <vector>

#include "GlobalTypes.h"
#include "shGameLogic.h"

namespace sh {
    class Game;

    class GameEvent {
    public:
        /**
         * Returns if the condition for the game event is met
         * @param numFascistCards number of fascist cards currently on the policy board
         * @param numLiberalCards number of liberal cards currently on the policy board
         * @param currentPolicy the policy to be played in this round
         * @return true if event condition is met, false otherwise
         */
        [[nodiscard]] virtual bool condition(const Game &gameState, GameEventTrigger trigger) const = 0;

        const GameEventType type;
    protected:
        [[nodiscard]] static auto getPolicyNumbers(const Game &gameState) -> std::pair<unsigned int, unsigned int>;
        explicit GameEvent(GameEventType type);
    };

    using GameEventPtr = std::shared_ptr<GameEvent>;
    using GameEventHandler = std::function<void(GameEventType)>;
    using RuleSet = std::vector<GameEventPtr>;

    class LiberalsWin : public GameEvent {
    public:
        LiberalsWin();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    class FascistsWin : public GameEvent {
    public:
        FascistsWin();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    class InvestigateLoyalty : public GameEvent {
    public:
        InvestigateLoyalty();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    class SpecialElection : public GameEvent {
    public:
        SpecialElection();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    class Execution : public GameEvent {
    public:
        Execution();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    class Veto : public GameEvent {
    public:
        Veto();
        [[nodiscard]] bool condition(const Game &gameState, GameEventTrigger trigger) const override;
    };

    enum class RuleSetType {
        Standard
    };

}

#endif //SHGAME_POLICYEVENT_H
