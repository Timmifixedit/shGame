//
// Created by tim on 16.06.20.
//

#include <gtest/gtest.h>
#include <deque>

#include "GlobalTypes.h"
#include "shGameLogic.h"
#include "CardRange.h"

TEST(card_range_test, view_only) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    std::vector<CardType> cardPile = game.getCardPile();
    CardRange topCards = game.drawCards(3);
    EXPECT_EQ(topCards.size(), 3);
    auto topCard = cardPile.rbegin();
    for (auto card : topCards) {
        EXPECT_EQ(card, *topCard);
        ++topCard;
    }

    EXPECT_NE(game.getCardPile(), cardPile);
    topCards.applyToGame();
    EXPECT_EQ(game.getCardPile(), cardPile);
}

TEST(card_range_test, discard_and_return) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    unsigned int nOfCards = game.getCardPile().size();
    CardRange topCards = game.drawCards(3);
    std::vector<CardType> discardedCards;
    while (topCards.size() > 1) {
        discardedCards.emplace_back(topCards(0));
        EXPECT_TRUE(topCards.discard(topCards(0)));
    }

    EXPECT_EQ(topCards.size(), 1);
    CardType remainingCard = *topCards.begin();
    EXPECT_TRUE(topCards.applyToGame());
    EXPECT_EQ(game.getCardPile().back(), remainingCard);
    EXPECT_EQ(game.getCardPile().size(), nOfCards - 2);
    EXPECT_EQ(game.getDiscardCardPile(), discardedCards);
}

TEST(card_range_test, policy_and_return) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    std::vector<CardType> cardPileOneLess = game.getCardPile();
    cardPileOneLess.pop_back();
    CardRange topCards = game.drawCards(3);
    CardType policy = topCards(0);
    EXPECT_TRUE(topCards.selectForPolicy(topCards(0)));
    EXPECT_EQ(topCards.size(), 2);
    EXPECT_TRUE(topCards.applyToGame());
    EXPECT_EQ(cardPileOneLess, game.getCardPile());
    EXPECT_TRUE(game.getDiscardCardPile().empty());
    EXPECT_EQ(game.getPolicies().find(policy)->second, 1);
    auto other = policy == sh::CardType::Fascist ? CardType::Liberal : CardType::Fascist;
    EXPECT_EQ(game.getPolicies().find(other)->second, 0);
}

TEST(car_range_test, discard_not_found) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    while (topCards.size() > 0) {
        EXPECT_TRUE(topCards.discard(topCards(0)));
    }

    EXPECT_FALSE(topCards.discard(sh::CardType::Fascist));
    EXPECT_FALSE(topCards.discard(sh::CardType::Liberal));
}

TEST(card_range_test, discard_applied) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    EXPECT_TRUE(topCards.applyToGame());
    EXPECT_FALSE(topCards.discard(topCards(0)));
}

TEST(card_range_test, policy_not_found) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    while (topCards.size() > 0) {
        EXPECT_TRUE(topCards.discard(topCards(0)));
    }

    EXPECT_FALSE(topCards.selectForPolicy(sh::CardType::Fascist));
    EXPECT_FALSE(topCards.selectForPolicy(sh::CardType::Liberal));
}

TEST(card_range_test, policy_applied) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    EXPECT_TRUE(topCards.applyToGame());
    EXPECT_FALSE(topCards.selectForPolicy(topCards(0)));
}

TEST(card_range_test, double_apply) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    EXPECT_TRUE(topCards.applyToGame());
    EXPECT_FALSE(topCards.applyToGame());
}

TEST(card_range_test, raii) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    std::vector<CardType> cardPile = game.getCardPile();
    {
        CardRange topCards = game.drawCards(3);
    }

    EXPECT_EQ(cardPile, game.getCardPile());
    EXPECT_TRUE(game.getDiscardCardPile().empty());
    EXPECT_EQ(game.getPolicies().find(CardType::Fascist)->second, 0);
    EXPECT_EQ(game.getPolicies().find(CardType::Liberal)->second, 0);
}

TEST(car_range_test, applied_status) {
    using namespace sh;
    Game game({"A", "B", "C", "D", "E", "F", "G"});
    CardRange topCards = game.drawCards(3);
    EXPECT_FALSE(topCards.alreadyApplied());
    topCards.applyToGame();
    EXPECT_TRUE(topCards.alreadyApplied());
}
