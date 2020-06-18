//
// Created by tim on 15.06.20.
//

#include <gtest/gtest.h>
#include <array>
#include <memory>
#include "GameEvent.h"

TEST(policy_event_test, event_type) {
    using PEventPtr = std::shared_ptr<sh::GameEvent>;
    std::array<PEventPtr, 6> events = {
            std::make_shared<sh::LiberalsWin>(),
            std::make_shared<sh::FascistsWin>(),
            std::make_shared<sh::InvestigateLoyalty>(),
            std::make_shared<sh::SpecialElection>(),
            std::make_shared<sh::Execution>(),
            std::make_shared<sh::Veto>()
    };

    EXPECT_EQ(events[0]->type, sh::GameEventType::LiberalsWin);
    EXPECT_EQ(events[1]->type, sh::GameEventType::FascistsWin);
    EXPECT_EQ(events[2]->type, sh::GameEventType::InvestigateLoyalty);
    EXPECT_EQ(events[3]->type, sh::GameEventType::SpecialElection);
    EXPECT_EQ(events[4]->type, sh::GameEventType::Execution);
    EXPECT_EQ(events[5]->type, sh::GameEventType::Veto);
}