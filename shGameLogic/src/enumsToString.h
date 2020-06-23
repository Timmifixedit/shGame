//
// Created by tim on 23.06.20.
//

#ifndef SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H
#define SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H

#include "GlobalTypes.h"
#include "shGameLogic.h"

#include <string>
#include <optional>

namespace sh::util::strings {
    std::string toString(sh::CardType cardType);
    std::string toString(sh::Player::GovernmentRole governmentRole);
    /**
     * Converts enum type to string
     * @param type
     * @param specialType display secret role if any (if player is Hitler or just fascist)
     * @return
     */
    std::string toString(sh::Player::Type type, bool specialType = false);

    /**
     * Converts a string to the appropriate enum type
     * @param string
     * @return the enum type or nothing if mismatch
     */
    auto toCardType(const std::string &string) -> std::optional<sh::CardType>;
    auto toGovRole(const std::string &string) -> std::optional<sh::Player::GovernmentRole>;
    auto toPlayerType(const std::string &string) -> std::optional<sh::Player::Type>;
}


#endif //SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H
