#include <stdexcept>
#include <utility>
#include <algorithm>

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

    bool Player::operator==(const Player &other) const {
        return name == other.name && role == other.role && type == other.type &&
            isDead() == other.isDead() && government == other.isInGovernment();
    }

    bool Player::operator!=(const Player &other) const {
        return !(*this == other);
    }

    bool Player::isInGovernment() const {
        return government;
    }

    void Player::elect() {
        if (!role.has_value()) {
            throw std::runtime_error("Player has no government role");
        }

        government = true;
    }

    void Player::removeFromGovernment() {
        role.reset();
        government = false;
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

    auto Game::getPolicies() const -> const std::map<CardType, unsigned int> &{
        return policyBoard;
    }

    void Game::restockCardPile() {
        constexpr auto TOTAL_NUM_CARDS = NUM_FAS_CARDS + NUM_LIB_CARDS;
        cardPile.clear();
        discardPile.clear();
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

    auto Game::getPlayerByName(const std::string &name) const -> std::optional<std::vector<Player>::const_iterator> {
        auto ret = std::find_if(players.begin(), players.end(), [name](const auto &a){return a.name == name;});
        if (ret != players.end()) {
            return ret;
        }

        return {};
    }

    auto Game::getPlayerByName(const std::string &name) -> std::optional<std::vector<Player>::iterator> {
        auto ret = std::find_if(players.begin(), players.end(), [name](const auto &a){return a.name == name;});
        if (ret != players.end()) {
            return ret;
        }

        return {};
    }

    auto Game::drawCards(unsigned int n) -> CardRange {
        return CardRange(*this, n);
    }

    auto Game::getCardPile() const -> const std::vector<CardType> & {
        return cardPile;
    }

    auto Game::getDiscardCardPile() const -> const std::vector<CardType> & {
        return discardPile;
    }

    auto Game::getPlayerByCurrentRole(Player::GovernmentRole role) const ->
        std::optional<std::vector<Player>::const_iterator> {
        auto ret = std::find_if(players.cbegin(), players.cend(),
                [role](const auto &a){return a.role == role;});
        if (ret == players.end()) {
            return {};
        }

        return ret;
    }

    auto Game::getPlayerByCurrentRole(Player::GovernmentRole role) -> std::optional<std::vector<Player>::iterator> {
        auto ret = std::find_if(players.begin(), players.end(),
                                [role](const auto &a){return a.role == role;});
        if (ret == players.end()) {
            return {};
        }

        return ret;
    }

    auto Game::setPlayerRole(const std::string &playerName, Player::GovernmentRole role) -> std::optional<SetRoleStatus> {
        using Role = Player::GovernmentRole;
        auto newRoleBearer = getPlayerByName(playerName);
        if (!newRoleBearer.has_value()) {
            return std::nullopt;
        }

        if ((*newRoleBearer)->isDead()) {
            return SetRoleStatus::PlayerIsDead;
        }

        bool invalidChancellor = role == Role::Chancellor && (*newRoleBearer)->isInGovernment();
        bool invalidPresident = newRoleBearer == getPlayerByCurrentRole(Player::GovernmentRole::President);
        if (invalidChancellor || invalidPresident) {
            return SetRoleStatus::Ineligible;
        }

        auto currentRoleBearer = getPlayerByCurrentRole(role);
        if (currentRoleBearer.has_value()) {
            (*currentRoleBearer)->role.reset();
        }

        (*newRoleBearer)->role = role;
        return SetRoleStatus::Success;
    }

    bool Game::killPlayer(const std::string &playerName) {
        auto player = getPlayerByName(playerName);
        if (!player.has_value()) {
            return false;
        }

        (*player)->kill();
        generateEventsAndNotify(GameEventTrigger::PlayerExecuted);

        return true;
    }

    void Game::setNextPresident() {
        auto currPres = presCheckpoint.has_value() ? presCheckpoint :
                getPlayerByCurrentRole(Player::GovernmentRole::President);
        if (presCheckpoint.has_value()) {
            presCheckpoint.reset();
        }

        if (!currPres.has_value()) {
            currPres = util::selectRandom(players);
        }

        std::size_t counter = players.size();
        do {
            if (++*currPres == players.end()) {
                currPres = players.begin();
            }

            if (--counter == 0) {
                throw std::runtime_error("All players are dead!");
            }
        } while ((*currPres)->isDead());

        setPlayerRole((*currPres)->name, Player::GovernmentRole::President);
    }

    void Game::subscribe(const GameEventHandler &handler) {
        handlers.emplace_back(handler);
    }

    void Game::notifyAll(GameEventType type) const {
        for (const auto &handler : handlers) {
            handler(type);
        }
    }

    void Game::generateEventsAndNotify(GameEventTrigger trigger) const {
        for (const auto &rule : rules) {
            if (rule->condition(*this, trigger)) {
                notifyAll(rule->type);
            }
        }
    }

    auto Game::getGovernment() -> std::vector<std::vector<Player>::iterator> {
        std::vector<std::vector<Player>::iterator> ret;
        ret.reserve(players.size());
        for (auto it = players.begin(); it != players.end(); ++it) {
            if (it->isInGovernment()) {
                ret.emplace_back(it);
            }
        }

        return ret;
    }

    void Game::electGovernment() {
        auto pres = getPlayerByCurrentRole(Player::GovernmentRole::President);
        auto chancellor = getPlayerByCurrentRole(Player::GovernmentRole::Chancellor);
        for (auto govPlayer : getGovernment()) {
            govPlayer->removeFromGovernment();
        }

        if (pres.has_value()) {
            setPlayerRole((*pres)->name, Player::GovernmentRole::President);
            (*pres)->elect();
        }

        if (chancellor.has_value()) {
            setPlayerRole((*chancellor)->name, Player::GovernmentRole::Chancellor);
            (*chancellor)->elect();
        }

        generateEventsAndNotify(GameEventTrigger::PlayerElected);
    }

    auto Game::getHitler() const -> std::vector<Player>::const_iterator {
        for (auto it = players.cbegin(); it != players.cend(); ++it) {
            if(it->type == Player::Type::Hitler) {
                return it;
            }
        }

        throw std::runtime_error("No player is Hitler. As if that's a bad thing...");
    }

    auto Game::setNextPresident(const std::string &playerName) -> std::optional<SetRoleStatus> {
        if (!presCheckpoint.has_value()) {
            presCheckpoint = getPlayerByCurrentRole(Player::GovernmentRole::President);
        }

        return setPlayerRole(playerName, Player::GovernmentRole::President);
    }

    void Game::advanceElectionTracker() {
        ++electionTracker;
        generateEventsAndNotify(GameEventTrigger::ElectionTrackerAdvanced);
    }

    unsigned int Game::getElectionTracker() const {
        return electionTracker;
    }

    void Game::resetElectionTracker() {
        electionTracker = 0;
    }

    CardType Game::playRandomPolicy() {
        CardRange topCard = drawCards(1);
        CardType ret = *topCard.begin();
        topCard.selectForPolicy(*topCard.begin());
        topCard.applyToGame();
        auto pres = getPlayerByCurrentRole(Player::GovernmentRole::President);
        for (auto &player : getGovernment()) {
            player->removeFromGovernment();
        }

        if (pres.has_value()) {
            setPlayerRole((*pres)->name, Player::GovernmentRole::President);
        }

        //TODO ignore presidential powers
        return ret;
    }
}
