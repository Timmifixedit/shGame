//Game rules can be found at https://secrethitler.com/assets/Secret_Hitler_Rules.pdf


#ifndef SHGAME_SHGAMELOGIC_H
#define SHGAME_SHGAMELOGIC_H
#include <vector>
#include <deque>
#include <string>
#include <array>

namespace sh {
    constexpr unsigned int NUM_LIB_CARDS = 6;
    constexpr unsigned int NUM_FAS_CARDS = 11;
    constexpr unsigned int MIN_NUM_PLAYERS = 5;
    constexpr unsigned int MAX_NUM_PLAYERS = 10;

    class Player {
    public:
        enum class Type{
            Fascist, Liberal, Hitler
        };

        Player(std::string name, Type type);

        /**
         * Marks the player as dead;
         */
        void kill();

        [[nodiscard]] bool isDead() const;

        const std::string name;
        const Type type;
    private:
        bool dead = false;
    };

    enum class CardType {
        Fascist, Liberal
    };

    class Game {
    public:
        /**
         * CTor Assigns roles to players according to the game rules. The card dec is created empty, the policy stack is initialized
         * @param players list of player names. Has to be lower than MAX_NUM_PLAYERS and higher than MIN_NUM_PLAYERS
         * @throws std::runtime_error if illegal number of players
         */
        explicit Game(const std::vector<std::string> &players);

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

    private:
        static std::vector<Player> assignPlayers(const std::vector<std::string> &pNames);
        const std::vector<Player> players;
        std::vector<CardType> policyBoard;
        std::deque<CardType> cardPool;
        std::size_t currentPlayer = 0;
    };
}

#endif //SHGAME_SHGAMELOGIC_H
