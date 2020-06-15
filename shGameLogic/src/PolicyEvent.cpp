//
// Created by tim on 15.06.20.
//

#include "PolicyEvent.h"
namespace sh {
    PolicyEvent::PolicyEvent(PolicyEvent::Type type) : type(type) {}

    LiberalsWin::LiberalsWin() : PolicyEvent(Type::LiberalsWin) {}

    bool LiberalsWin::condition(unsigned int , unsigned int numLiberalCards, CardType currentPolicy) const {
        constexpr unsigned int NUM_LIB_CARDS_WIN = 5;
        return currentPolicy == CardType::Liberal && numLiberalCards == NUM_LIB_CARDS_WIN;
    }

    FascistsWin::FascistsWin() : PolicyEvent(Type::FascistsWin) {}

    bool FascistsWin::condition(unsigned int numFascistCards, unsigned int , CardType currentPolicy) const {
        constexpr unsigned int NUM_FAS_CARDS_WIN = 6;
        return currentPolicy == CardType::Fascist && numFascistCards == NUM_FAS_CARDS_WIN;
    }

    InvestigateLoyalty::InvestigateLoyalty() : PolicyEvent(Type::InvestigateLoyalty) {}

    bool InvestigateLoyalty::condition(unsigned int numFascistCards, unsigned int ,
                                       CardType currentPolicy) const {
        constexpr unsigned int NUM_FAS_CARDS_LOYAL = 2;
        return currentPolicy == CardType::Fascist && numFascistCards == NUM_FAS_CARDS_LOYAL;
    }

    SpecialElection::SpecialElection() : PolicyEvent(Type::SpecialElection) {}

    bool SpecialElection::condition(unsigned int numFascistCards, unsigned int ,
                                    CardType currentPolicy) const {
        constexpr unsigned int NUM_FAS_CARDS_ELECTION = 3;
        return currentPolicy == CardType::Fascist && numFascistCards == NUM_FAS_CARDS_ELECTION;
    }

    Execution::Execution() : PolicyEvent(Type::Execution) {}

    bool Execution::condition(unsigned int numFascistCards, unsigned int ,
                              CardType currentPolicy) const {
        constexpr unsigned int NUM_FAS_CARDS_EXEC = 4;
        return currentPolicy == CardType::Fascist && numFascistCards == NUM_FAS_CARDS_EXEC;
    }

    Veto::Veto() : PolicyEvent(Type::Veto) {}

    bool Veto::condition(unsigned int numFascistCards, unsigned int , CardType ) const {
        constexpr unsigned int NUM_FAS_CARDS_VETO = 5;
        return numFascistCards >= NUM_FAS_CARDS_VETO;
    }
}
