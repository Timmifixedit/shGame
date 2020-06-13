#include <stdexcept>
#include "shGameLogic.h"
#include "util.h"
namespace sh {
    Player::Player(std::string name, sh::Player::Type type) : name(std::move(name)), type(type) {}

    bool Player::isDead() const {
        return dead ;
    }

    void Player::kill() {
        dead = true;
    }

    Game::Game(const std::vector<std::string> &players) : players(assignPlayers(players)),
        cardPool(NUM_FAS_CARDS, CardType::Fascist) {
        for(unsigned int i = 0; i < NUM_LIB_CARDS; ++i) {
            cardPool.emplace_back(CardType::Liberal);
        }
    }

    unsigned int Game::numberOfLiberals(unsigned int numPlayers) {
        return numPlayers / 2 + 1;
    }

    std::vector<Player> Game::assignPlayers(const std::vector<std::string> &pNames) {
        if (pNames.size() < MIN_NUM_PLAYERS || pNames.size() > MAX_NUM_PLAYERS) {
            throw std::runtime_error(std::string("Number of players has to be between ") +
                                     std::to_string(MIN_NUM_PLAYERS) + " and " + std::to_string(MAX_NUM_PLAYERS));
        }

        std::vector<Player> ret;
        ret.reserve(pNames.size());
        unsigned int nLibPlayers = numberOfLiberals(pNames.size());
        std::deque<Player::Type> pTypes(nLibPlayers, Player::Type::Liberal);
        pTypes.emplace_back(Player::Type::Hitler);
        for (std::size_t i = 0; i < pNames.size() - nLibPlayers - 1; ++i) {
            pTypes.emplace_back(Player::Type::Fascist);
        }

        for(const auto &player : pNames) {
            auto pType = util::selectRandom(pTypes);
            ret.emplace_back(player, *pType);
            pTypes.erase(pType);
        }

        return ret;
    }

    auto Game::getPlayers() const -> const std::vector<Player> & {
        return players;
    }
}
