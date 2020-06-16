//
// Created by tim on 16.06.20.
//

#ifndef SHGAME_CARDRANGE_H
#define SHGAME_CARDRANGE_H

#include <vector>
#include <optional>
#include <deque>

#include "shGameLogic.h"
#include "GlobalTypes.h"
#include "PolicyEvent.h"

namespace sh{
    class Game;
    class CardRange {
    public:
        CardRange(Game &game, unsigned int n);

        /**
         * Gets the cards contained in the range
         * @return
         */
        [[nodiscard]] auto content() const -> const std::deque<CardType>&;

        /**
         * Marks a card of the selected type as discarded
         * @param card type of the card to be discarded
         * @return true if card was discarded successfully, false if card of specified type cannot be found or if range
         * was already applied to game
         */
        bool discard(CardType card);

        /**
         * Selects a card to be added as policy
         * @param card type of the card to be added as policy
         * @return true if card was successfully marked, false if card of specified type cannot be found or a policy
         * has already been chosen or if range was already applied to game
         */
        bool selectForPolicy(CardType card);

        /**
         * Applies the card range to the corresponding game. The card selected as policy is added to the policy board,
         * cards marked for discard are added to the discard pile. Remaining cards are put back to the card pile
         * @return false if this method has already been called, true otherwise
         */
        bool applyToGame();

        /**
         * Whether the card range has been applied to the game
         * @return true if already applied, false otherwise
         */
        [[nodiscard]] bool alreadyApplied() const;

        /**
         * If changes to board were not applied, cards are put back to card pile of Game
         */
        ~CardRange();

        [[nodiscard]] auto begin() const -> std::deque<CardType>::const_iterator;
        [[nodiscard]] auto end() const -> std::deque<CardType>::const_iterator;

        /**
         * Returns the number of cards in the CardRange, discarded and policy cards are NOT included
         * @return
         */
        [[nodiscard]] std::size_t size() const;

        /**
         * Index based read access to content of CardRange, discarded and policy cards are NOT included
         * @param i index of card, no range checks are performed!
         * @return
         */
        CardType operator()(std::size_t i) const;

    private:
        Game &game;
        std::vector<CardType> initialState;
        std::deque<CardType> cards;
        std::vector<CardType> discarded;
        std::optional<CardType> policy = std::nullopt;
        bool applied = false;
    };
}


#endif //SHGAME_CARDRANGE_H
