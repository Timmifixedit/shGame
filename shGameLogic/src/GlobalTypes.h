//
// Created by tim on 15.06.20.
//

#ifndef SHGAME_GLOBALTYPES_H
#define  SHGAME_GLOBALTYPES_H
#include <functional>

namespace sh{
    /**
     * Type of policy cards
     */
    enum class CardType {
        Fascist, Liberal
    };

    enum class PolicyEventType {
        LiberalsWin, FascistsWin,
        InvestigateLoyalty, Execution,
        PolicyPeak, SpecialElection, Veto
    };

    using PolicyEventHandler = std::function<void(PolicyEventType)>;

    enum class SetRoleStatus {
        PlayerIsDead, Ineligible, Success
    };
}
#endif //SHGAME_GLOBALTYPES_H
