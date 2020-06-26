//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_GLOBALTYPES_H
#define SHGAME_GLOBALTYPES_H

namespace sh {
    /**
     * Type of policy cards
     */
    enum class CardType {
        Fascist, Liberal
    };

    enum class SetRoleStatus {
        PlayerIsDead, Ineligible, Success
    };

    enum class GameEventTrigger {
        FascistPolicyPlayed, LiberalPolicyPlayed, PlayerExecuted, PlayerElected, ElectionTrackerAdvanced
    };

    enum class GameEventType {
        LiberalsWin, FascistsWin,
        InvestigateLoyalty, Execution,
        PolicyPeak, SpecialElection, Veto, RandomPolicy
    };
}
#endif //SHGAME_GLOBALTYPES_H
