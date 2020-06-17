//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_GLOBALTYPES_H
#define  SHGAME_GLOBALTYPES_H

namespace sh{
    /**
     * Type of policy cards
     */
    enum class CardType {
        Fascist, Liberal
    };

    enum class PolicyEvent {
        LiberalsWin, FascistsWin,
        InvestigateLoyalty, Execution,
        PolicyPeak, SpecialElection, Veto
    };

    enum class SetRoleStatus {
        PlayerIsDead, Ineligible, Success
    };
}
#endif //SHGAME_GLOBALTYPES_H
