//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_POLICYEVENT_H
#define SHGAME_POLICYEVENT_H

#include "GlobalTypes.h"

namespace sh {

    class PolicyEvent {
    public:
        enum class Type {
            LiberalsWin, FascistsWin,
            InvestigateLoyalty, Execution,
            PolicyPeak, SpecialElection, Veto
        };

        /**
         * Returns if the condition for the policy event is met
         * @param numFascistCards number of fascist cards currently on the policy board
         * @param numLiberalCards number of liberal cards currently on the policy board
         * @param currentPolicy the policy to be played in this round
         * @return true if event condition is met, false otherwise
         */
        [[nodiscard]] virtual bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const = 0;

        const Type type;
    protected:
        explicit PolicyEvent(Type type);
    };

    class LiberalsWin : public PolicyEvent {
    public:
        LiberalsWin();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };

    class FascistsWin : public PolicyEvent {
    public:
        FascistsWin();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };

    class InvestigateLoyalty : public PolicyEvent {
    public:
        InvestigateLoyalty();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };

    class SpecialElection : public PolicyEvent {
    public:
        SpecialElection();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };

    class Execution : public PolicyEvent {
    public:
        Execution();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };

    class Veto : public PolicyEvent {
    public:
        Veto();
        [[nodiscard]] bool condition(unsigned int numFascistCards, unsigned int numLiberalCards, CardType currentPolicy) const override;
    };
}

#endif //SHGAME_POLICYEVENT_H
