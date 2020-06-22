//
// Created by tim on 15.06.20.
//

#include <gtest/gtest.h>
#include <array>
#include <memory>
#include <string>

#include "GameEvent.h"
#include "shGameLogic.h"
#include "rules.h"
#include "CardRange.h"


using NameList = std::vector<std::string>;
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

/**
 * Plays a number of cards of a certain type
 * @param n number of cards to play
 * @param cardType type of the played cards
 * @param game game object to play the cards in
 */
static void playNCArds(unsigned int n, sh::CardType cardType, sh::Game &game){
    for (auto i = 0u; i < n; ++i) {
        sh::CardRange allCards = game.drawCards(game.getCardPile().size());
        EXPECT_TRUE(allCards.selectForPolicy(cardType));
        EXPECT_TRUE(allCards.applyToGame());
    }
}

/***
 * Creates an event handler for a game event that sets a success-variable to true only if the specified event occurs
 * at the right time. If it occurs any other time, the handler will generate a failure
 * @param eventType the expected event type
 * @param succeedNow reference to a bool which tells the handler exactly when the event has to occur
 * @param success reference to a bool which is set true by the handler if the event occurs at the correct time
 * @return
 */
static sh::GameEventHandler succeedOnEventNow(sh::GameEventType eventType, const bool &succeedNow, bool &success) {
    return [&success, &succeedNow, eventType](sh::GameEventType type) {
        if (!succeedNow && type == eventType) {
            FAIL() << "in" << ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name() << " " <<
                        ::testing::UnitTest::GetInstance()->current_test_info()->name();
        } else if (type == eventType) {
            success = true;
        }
    };
}

/**
 * Creates an event handler for a game event that generates a failure if the specified event occurs
 * @param eventType the event type that must not occur
 * @return
 */
static sh::GameEventHandler failOnEvent(sh::GameEventType eventType) {
    return [eventType](sh::GameEventType type) {
        if (type == eventType) {
            FAIL() << "in" << ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name() << " " <<
                   ::testing::UnitTest::GetInstance()->current_test_info()->name();
        }
    };
}

TEST(game_event_test, liberals_win) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::LiberalsWin, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    game.subscribe(failOnEvent(GameEventType::InvestigateLoyalty));
    game.subscribe(failOnEvent(GameEventType::SpecialElection));
    game.subscribe(failOnEvent(GameEventType::Execution));
    game.subscribe(failOnEvent(GameEventType::Veto));
    playNCArds(4, CardType::Liberal, game);
    succeedNow = true;
    playNCArds(1, CardType::Liberal, game);

    EXPECT_TRUE(success);
}

TEST(game_event_test, liberals_kill_hitler) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = true;
    game.subscribe(succeedOnEventNow(GameEventType::LiberalsWin, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    game.subscribe(failOnEvent(GameEventType::InvestigateLoyalty));
    game.subscribe(failOnEvent(GameEventType::SpecialElection));
    game.subscribe(failOnEvent(GameEventType::Execution));
    game.subscribe(failOnEvent(GameEventType::Veto));
    EXPECT_TRUE(game.killPlayer(game.getHitler()->name));
    EXPECT_TRUE(success);
}

TEST(game_event_test, fascists_win) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::FascistsWin, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    playNCArds(5, CardType::Fascist, game);
    succeedNow = true;
    playNCArds(1, CardType::Fascist, game);

    EXPECT_TRUE(success);
}


TEST(game_event_test, hilter_wins) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;

    game.subscribe(succeedOnEventNow(GameEventType::FascistsWin, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    game.subscribe(failOnEvent(GameEventType::Veto));
    game.subscribe(failOnEvent(GameEventType::Execution));
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

TEST(game_event_test, investigate_loyalty) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::InvestigateLoyalty, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    game.subscribe(failOnEvent(GameEventType::SpecialElection));
    game.subscribe(failOnEvent(GameEventType::Execution));
    game.subscribe(failOnEvent(GameEventType::Veto));
    playNCArds(1, CardType::Fascist, game);
    succeedNow = true;
    playNCArds(1, CardType::Fascist, game);

    EXPECT_TRUE(success);
}

TEST(game_event_test, special_election) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::SpecialElection, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    game.subscribe(failOnEvent(GameEventType::Execution));
    game.subscribe(failOnEvent(GameEventType::Veto));
    playNCArds(2, CardType::Fascist, game);
    succeedNow = true;
    playNCArds(1, CardType::Fascist, game);

    EXPECT_TRUE(success);
}

TEST(game_event_test, execution) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::Execution, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    game.subscribe(failOnEvent(GameEventType::Veto));
    playNCArds(3, CardType::Fascist, game);
    succeedNow = true;
    playNCArds(1, CardType::Fascist, game);

    EXPECT_TRUE(success);
}

TEST(game_event_test, veto) {
    using namespace sh;
    Game game(NameList{"A", "B", "C", "D", "E", "F", "G"}, createRuleSet(RuleSetType::Standard));
    bool success = false;
    bool succeedNow = false;
    game.subscribe(succeedOnEventNow(GameEventType::Veto, succeedNow, success));
    game.subscribe(failOnEvent(GameEventType::LiberalsWin));
    game.subscribe(failOnEvent(GameEventType::FascistsWin));
    playNCArds(4, CardType::Fascist, game);
    succeedNow = true;
    playNCArds(1, CardType::Fascist, game);

    EXPECT_TRUE(success);
}

