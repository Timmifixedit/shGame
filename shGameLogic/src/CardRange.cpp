//
// Created by tim on 16.06.20.
//

#include "CardRange.h"
#include <string>
#include <algorithm>
namespace sh {

    CardRange::CardRange(Game &game, unsigned int n) : game(game) {
        std::size_t totNumCards = this->game.cardPile.size() + this->game.discardPile.size();
        if (n > totNumCards) {
            throw std::runtime_error(std::string("Cannot create card range of size ") + std::to_string(n)
                                    + "! Game has only " + std::to_string(totNumCards) + " left!");
        }

        if(n > this->game.cardPile.size()) {
            this->game.restockCardPile();
        }

        cards = std::deque<CardType>(this->game.cardPile.end() - n, this->game.cardPile.end());
        this->game.cardPile.erase(this->game.cardPile.end() - n, this->game.cardPile.end());
    }

    auto CardRange::content() const -> const std::deque<CardType> & {
        return cards;
    }

    bool CardRange::discard(CardType card) {
        auto res = std::find(cards.begin(), cards.end(), card);
        if (res != cards.end()) {
            discarded.emplace_back(card);
            cards.erase(res);
            return true;
        }

        return false;
    }

    bool CardRange::selectForPolicy(CardType card) {
        if (policy.has_value()) {
            return false;
        }

        auto res = std::find(cards.begin(), cards.end(), card);
        if (res != cards.end()) {
            policy.emplace(card);
            cards.erase(res);
            return true;
        }

        return false;
    }

    bool CardRange::applyToGame() {
        if (applied) {
            return false;
        }

        applied = true;
        if (policy.has_value()) {
            game.policyBoard.find(*policy)->second++;
        }

        for (auto card : discarded) {
            game.discardPile.emplace_back(card);
        }

        for (auto card : cards) {
            game.cardPile.emplace_back(card);
        }

        return true;
    }

    CardRange::~CardRange() {
        applyToGame();
    }
}