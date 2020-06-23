//
// Created by tim on 23.06.20.
//

#ifndef SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H
#define SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H

#include "GlobalTypes.h"
#include <shGameLogic.h>

#include <string>

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
}


#endif //SECRETHITLERGAMELOGIC_ENUMSTOSTRING_H
