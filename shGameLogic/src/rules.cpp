//
// Created by tim on 18.06.20.
//

#include "rules.h"
namespace sh {
    auto createRuleSet(RuleSetType type) -> RuleSet {
        using std::make_shared;
        switch (type) {
            case RuleSetType::Standard:
                return {make_shared<LiberalsWin>(), make_shared<FascistsWin>(), make_shared<SpecialElection>(),
                        make_shared<InvestigateLoyalty>(), make_shared<Execution>(), make_shared<Veto>()};
        }

        throw std::runtime_error("Unknown rule set type");
    }
}