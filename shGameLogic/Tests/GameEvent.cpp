//
// Created by tim on 15.06.20.
//

#include <gtest/gtest.h>
#include <array>
#include <memory>
#include "GameEvent.h"
#include "shGameLogic.h"
#include "rules.h"
#include "CardRange.h"

TEST(game_event_test, event_type) {
    std::array<sh::GameEventPtr , 6> events = {
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

TEST(game_event_test, liberals_win) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    auto handler = [&success, &succeedNow](GameEventType type) {
        if (type == GameEventType::LiberalsWin && succeedNow) {
            success = true;
        } else {
            FAIL();
        }
    };

    game.subscribe(handler);
    for (int i = 0; i < 5; ++i) {
        if (i == 4) {
            succeedNow = true;
        }

        CardRange allCards = game.drawCards(game.getCardPile().size());
        EXPECT_TRUE(allCards.selectForPolicy(CardType::Liberal));
        EXPECT_TRUE(allCards.applyToGame());
    }

    EXPECT_TRUE(success);
}

TEST(game_event_test, liberals_kill_hitler) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    auto handler = [&success](GameEventType type) {
        if (type == GameEventType::LiberalsWin) {
            success = true;
        } else {
            FAIL();
        }
    };

    game.subscribe(handler);
    EXPECT_TRUE(game.killPlayer(game.getHitler()->name));
    EXPECT_TRUE(success);
}

TEST(game_event_test, fascists_win) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    auto handler = [&success, &succeedNow](GameEventType type) {
        if (!succeedNow && type == GameEventType::FascistsWin) {
            FAIL();
        } else if (type == GameEventType::FascistsWin && succeedNow) {
            success = true;
        }
    };

    game.subscribe(handler);
    for (int i = 0; i < 6; ++i) {
        if (i == 5) {
            succeedNow = true;
        }

        CardRange allCards = game.drawCards(game.getCardPile().size());
        EXPECT_TRUE(allCards.selectForPolicy(CardType::Fascist));
        EXPECT_TRUE(allCards.applyToGame());
    }

    EXPECT_TRUE(success);
}

TEST(game_event_test, hilter_wins) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    auto handler = [&success, &succeedNow](GameEventType type) {
        if (!succeedNow && type == GameEventType::FascistsWin) {
            FAIL();
        } else if (type == GameEventType::FascistsWin && succeedNow) {
            success = true;
        }
    };

    game.subscribe(handler);
    for (int i = 0; i < 3; ++i) {
        EXPECT_TRUE(game.setPlayerRole(game.getHitler()->name, Player::GovernmentRole::Chancellor));
        game.electGovernment();
        auto notHitler = game.getHitler();
        if (++notHitler == game.getPlayers().end()) {
            notHitler = game.getPlayers().begin();
        }

        EXPECT_TRUE(game.setPlayerRole(notHitler->name, Player::GovernmentRole::Chancellor));
        game.electGovernment();
        CardRange allCards = game.drawCards(game.getCardPile().size());
        EXPECT_TRUE(allCards.selectForPolicy(CardType::Fascist));
        EXPECT_TRUE(allCards.applyToGame());
    }

    succeedNow = true;
    EXPECT_TRUE(game.setPlayerRole(game.getHitler()->name, Player::GovernmentRole::Chancellor));
    game.electGovernment();
    EXPECT_TRUE(success);
}