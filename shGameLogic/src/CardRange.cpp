//
// Created by tim on 16.06.20.
//

#include "CardRange.h"
#include <string>
#include <algorithm>
namespace sh {

    CardRange::CardRange(Game &game, unsigned int n) : game(game) {
        std::size_t drawableCards = this->game.cardPile.size() + this->game.discardPile.size();
        if (n > drawableCards) {
            throw std::runtime_error(std::string("Cannot create card range of size ") + std::to_string(n)
                                     + "! Game has only " + std::to_string(drawableCards) + " left!");
        }

        if(n > this->game.cardPile.size()) {
            this->game.restockCardPile();
        }

        initialState = std::vector<CardType>(this->game.cardPile.rbegin(), this->game.cardPile.rbegin() + n);
        cards = std::deque<CardType>(this->game.cardPile.rbegin(), this->game.cardPile.rbegin() + n);
        this->game.cardPile.erase(this->game.cardPile.end() - n, this->game.cardPile.end());
    }

    auto CardRange::content() const -> const std::deque<CardType> & {
        return cards;
    }

    bool CardRange::discard(CardType card) {
        if (applied) {
            return false;
        }

        auto res = std::find(cards.begin(), cards.end(), card);
        if (res != cards.end()) {
            discarded.emplace_back(card);
            cards.erase(res);
            return true;
        }

        return false;
    }

    bool CardRange::selectForPolicy(CardType card) {
        if (policy.has_value() || applied) {
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

    auto CardRange::applyToGame() -> std::pair<bool, std::optional<PolicyEvent>> {
        if (applied) {
            return {false, std::nullopt};
        }

        applied = true;
        if (policy.has_value()) {
            game.policyBoard.find(*policy)->second++;
        }

        for (auto card : discarded) {
            game.discardPile.emplace_back(card);
        }

        for (auto it = cards.rbegin(); it != cards.rend(); ++it) {
            game.cardPile.emplace_back(*it);
        }

        return {true, getOccuringEvent()};
    }

    CardRange::~CardRange() {
        if (!applied) {
            for (auto it = initialState.rbegin(); it != initialState.rend(); ++it) {
                game.cardPile.emplace_back(*it);
            }
        }
    }

    auto CardRange::begin() const -> std::deque<CardType>::const_iterator {
        return cards.cbegin();
    }

    auto CardRange::end() const -> std::deque<CardType>::const_iterator {
        return cards.cend();
    }

    CardType CardRange::operator()(std::size_t i) const {
        return cards[i];
    }

    std::size_t CardRange::size() const {
        return cards.size();
    }

    bool CardRange::alreadyApplied() const {
        return applied;
    }

    auto CardRange::getOccuringEvent() const -> std::optional<PolicyEvent> {
        constexpr unsigned int NUM_LIB_CARDS_WIN = 5;
        constexpr unsigned int NUM_FAS_CARDS_WIN = 6;
        constexpr unsigned int NUM_FAS_CARDS_VETO = 5;
        constexpr unsigned int NUM_FAS_CARDS_EXEC = 4;
        constexpr unsigned int NUM_FAS_CARDS_ELECTION = 3;
        constexpr unsigned int NUM_FAS_CARDS_LOYAL = 2;
        if (!policy.has_value()) {
            return {};
        }

        unsigned int criticalCards = game.getPolicies().find(*policy)->second;
        switch (*policy) {
            case CardType::Fascist:
                if (criticalCards == NUM_FAS_CARDS_WIN) {
                    return PolicyEvent::FascistsWin;
                } else if (criticalCards >= NUM_FAS_CARDS_VETO) {
                    return PolicyEvent::Veto;
                } else if (criticalCards == NUM_FAS_CARDS_EXEC) {
                    return PolicyEvent::Execution;
                } else if (criticalCards == NUM_FAS_CARDS_ELECTION) {
                    return PolicyEvent::SpecialElection;
                } else if (criticalCards == NUM_FAS_CARDS_LOYAL) {
                    return PolicyEvent::InvestigateLoyalty;
                }

                break;
            case CardType::Liberal:
                if (criticalCards == NUM_LIB_CARDS_WIN) {
                    return PolicyEvent::LiberalsWin;
                }

                break;
        }

        return {};
    }
}