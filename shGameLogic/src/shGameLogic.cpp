#include <stdexcept>
#include <utility>
#include "shGameLogic.h"
#include "util.h"
namespace sh {
    Player::Player(std::string name, sh::Player::Type type, std::optional<GovernmentRole> role) :
        name(std::move(name)), type(type), role(std::move(role)) {}

    bool Player::isDead() const {
        return dead ;
    }

    void Player::kill() {
        dead = true;
    }

    Game::Game(const std::vector<std::string> &players) : players(assignPlayers(players)),
                                                          policyBoard() {
        restockCardPile();
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

        util::selectRandom(ret)->role = Player::GovernmentRole::President;
        return ret;
    }

    auto Game::getPlayers() const -> const std::vector<Player> & {
        return players;
    }

    auto Game::getPolicies() const -> const std::map<CardType, unsigned int> &{
        return policyBoard;
    }

    void Game::restockCardPile() {
        constexpr auto TOTAL_NUM_CARDS = NUM_FAS_CARDS + NUM_LIB_CARDS;
        cardPile.clear();
        cardPile.reserve(TOTAL_NUM_CARDS);
        unsigned int libCards = NUM_LIB_CARDS - policyBoard.find(CardType::Liberal)->second;
        unsigned int fascistCards = NUM_FAS_CARDS - policyBoard.find(CardType::Fascist)->second;
        std::deque<CardType> reservoire(libCards, CardType::Liberal);
        for (unsigned int i = 0; i < fascistCards; ++i) {
            reservoire.emplace_back(CardType::Fascist);
        }

        while (!reservoire.empty()) {
            const auto card = util::selectRandom(reservoire);
            cardPile.emplace_back(*card);
            reservoire.erase(card);
        }
    }


}
