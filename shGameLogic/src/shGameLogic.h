//Game rules can be found at https://secrethitler.com/assets/Secret_Hitler_Rules.pdf


#ifndef SHGAME_SHGAMELOGIC_H
#define SHGAME_SHGAMELOGIC_H
#include <vector>
#include <deque>
#include <string>
#include <array>
#include <map>
#include <optional>

#include "GlobalTypes.h"
#include "CardRange.h"
#include "GameEvent.h"
#include "util.h"

namespace sh {
    constexpr unsigned int NUM_LIB_CARDS = 6;
    constexpr unsigned int NUM_FAS_CARDS = 11;
    constexpr unsigned int MIN_NUM_PLAYERS = 5;
    constexpr unsigned int MAX_NUM_PLAYERS = 10;
    class CardRange;
    class GameEvent;

    using GameEventPtr = std::shared_ptr<GameEvent>;
    using GameEventHandler = std::function<void(GameEventType)>;
    using RuleSet = std::vector<GameEventPtr>;

    /**
     * Represents a player
     */
    class Player {
    public:
        enum class Type {
            Fascist, Liberal, Hitler
        };

        enum class GovernmentRole {
            President, Chancellor
        };

        Player(std::string name, Type type, std::optional<GovernmentRole> role = std::nullopt);

        /**
         * Marks the player as dead;
         */
        void kill();

        void elect();

        void removeFromGovernment();

        [[nodiscard]] bool isDead() const;
        [[nodiscard]] bool isInGovernment() const;

        bool operator==(const Player &other) const;
        bool operator!=(const Player &other) const;

        const std::string name;
        const Type type;
        std::optional<GovernmentRole> role;
    private:
        bool dead = false;
        bool government = false;
    };


    class Game {
    public:
        /**
         * CTor Assigns roles to players according to the game rules. The card dec is created empty, the policy stack is initialized
         * @param players list of player names. Has to be lower than MAX_NUM_PLAYERS and higher than MIN_NUM_PLAYERS
         * @throws std::runtime_error if illegal number of players
         */
        template<typename T /*= std::vector<std::string>*/>
        Game(const T &players, RuleSet rules) :
                players(assignPlayers(players)), rules(std::move(rules)),
                policyBoard({{CardType::Liberal, 0}, {CardType::Fascist, 0}}) {
            static_assert(std::is_array<T>::value || util::customTypeTraits::is_iterable<T>::value, "Argument is not iterable");
                    restockCardPile();
                }

        /**
         * Returns the number of liberal players in the game according to the game rules
         * @param numPlayers total number of player playing the game
         * @return number of fascist players in the game
         */
        static unsigned int numberOfLiberals(unsigned int numPlayers);

        /**
         * Returns vector of all players
         * @return
         */
        [[nodiscard]] auto getPlayers() const -> const std::vector<Player> &;

        /**
         * Returns the number of liberal and fascist policies that where played previously
         * @return Map of CardType -> number of played policies
         */
        [[nodiscard]] auto getPolicies() const -> const std::map<CardType, unsigned int> &;

        /**
         * Returns player with matching name
         * @param name name of the plyer
         * @return corresponding player or nothing if no player with specified name was found
         */
        [[nodiscard]] auto getPlayerByName(const std::string &name) const -> std::optional<std::vector<Player>::const_iterator>;
        [[nodiscard]] auto getPlayerByName(const std::string &name) -> std::optional<std::vector<Player>::iterator>;

        /**
         * Gets the top n cards from the card pile.
         * @param n Number of cards to view
         * @return Top card on the card pile as CardRange
         */
        auto drawCards(unsigned int n) -> CardRange;

        /**
         * Gets the current cards on the card pile
         * @return
         */
        [[nodiscard]] auto getCardPile() const -> const std::vector<CardType>&;

        /**
         * Gets the current cards on the discard pile
         * @return
         */
        [[nodiscard]] auto getDiscardCardPile() const -> const std::vector<CardType>&;

        /**
         * Returns an iterator to the player with the specified role
         * @param role desired role
         * @return
         */
        auto getPlayerByCurrentRole(Player::GovernmentRole role) const -> std::optional<std::vector<Player>::const_iterator>;
        auto getPlayerByCurrentRole(Player::GovernmentRole role) -> std::optional<std::vector<Player>::iterator>;

        auto getGovernment() -> std::vector<std::vector<Player>::iterator>;

        /**
         * Sets the role of the player with the specified role
         * @param playerName name of the player
         * @param role desired role
         * @return Success if role was assigned, PlayerIsDead if player cannot be assigned a role since they are dead,
         * Ineligible if trying to elect a player that is ineligible according to the game rules, nothing if player
         * cannot be found by the specified name
         */
        auto setPlayerRole(const std::string &playerName, Player::GovernmentRole role) -> std::optional<SetRoleStatus>;

        /**
         * Marks the specified player as dead
         * @param playerName name of the player
         * @return true if player was successfully killed, false if player could not be found by the name
         */
        bool killPlayer(const std::string &playerName);

        /**
         * Sets the next player in row (who is still alive) as president. If no player is president yet, one is
         * randomly chosen
         */
        void setNextPresident();

        auto setNextPresident(const std::string &playerName) -> std::optional<SetRoleStatus>;

        /**
         * Subscribes the given event handler to game events
         * @param handler the handler handling the event
         */
        void subscribe(const GameEventHandler &handler);

        void electGovernment();

        void advanceElectionTracker();

        void resetElectionTracker();

        CardType playRandomPolicy();

        [[nodiscard]] unsigned int getElectionTracker() const;

        auto getHitler() const -> std::vector<Player>::const_iterator;

    private:
        friend class CardRange;

        template<typename T>
        static std::vector<Player> assignPlayers(const T &pNames) {
            std::vector<std::string> names(std::begin(pNames), std::end(pNames));
            return assignPlayers(names);
        }
        static std::vector<Player> assignPlayers(const std::vector<std::string> &pNames);
        void restockCardPile();
        void notifyAll(GameEventType type) const;
        void generateEventsAndNotify(GameEventTrigger trigger) const;
        std::vector<Player> players;
        RuleSet rules;
        std::map<CardType, unsigned int> policyBoard;
        std::vector<CardType> cardPile;
        std::vector<CardType> discardPile;
        std::vector<GameEventHandler> handlers;
        std::optional<std::vector<Player>::iterator> presCheckpoint;
        unsigned int electionTracker = 0;
    };
}

#endif //SHGAME_SHGAMELOGIC_H
