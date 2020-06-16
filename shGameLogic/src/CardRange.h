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

namespace sh{
    class Game;
    class CardRange {
    public:
        CardRange(Game &game, unsigned int n);

        /**
         * Gets the cards contained in the range
         * @return
         */
        auto content() const -> const std::deque<CardType>&;

        /**
         * Marks a card of the selected type as discarded
         * @param card type of the card to be discarded
         * @return true if card was discarded successfully, false if card of specified type cannot be found
         */
        bool discard(CardType card);

        /**
         * Selects a card to be added as policy
         * @param card type of the card to be added as policy
         * @return true if card was successfully marked, false if card of specified type cannot be found or a policy
         * has already been chosen
         */
        bool selectForPolicy(CardType card);

        /**
         * Applies the card range to the corresponding game. The card selected as policy is added to the policy board,
         * cards marked for discard are added to the discard pile. Remaining cards are put back to the card pile
         * @return false if this method has already been called, true otherwise
         */
        bool applyToGame();

        /**
         * Applies the card range to the corresponding game (see applyToGame) to ensure that the number of cards in the
         * game stays constant
         */
        ~CardRange();

    private:
        Game &game;
        std::deque<CardType> cards;
        std::vector<CardType> discarded;
        std::optional<CardType> policy = std::nullopt;
        bool applied = false;
    };
}


#endif //SHGAME_CARDRANGE_H
