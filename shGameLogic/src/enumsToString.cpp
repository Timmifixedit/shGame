//
// Created by tim on 23.06.20.
//

#include "enumsToString.h"

namespace sh::util::strings {
    std::string toString(sh::CardType cardType) {
        switch (cardType) {
            case CardType::Fascist:
                return "Fascist";
            case CardType::Liberal:
                return "Liberal";
        }

        throw std::runtime_error("Enum out of bounds");
    }

    std::string toString(sh::Player::GovernmentRole governmentRole) {
        switch (governmentRole) {
            case Player::GovernmentRole::President:
                return "President";
            case Player::GovernmentRole::Chancellor:
                return "Chancellor";
        }

        throw std::runtime_error("Enum out of bounds");
    }

    std::string toString(sh::Player::Type type, bool specialType) {
        switch (type) {
            case Player::Type::Fascist:
                return "Fascist";
            case Player::Type::Liberal:
                return "Liberal";
            case Player::Type::Hitler:
                return specialType ? "Hitler" : "Fascist";
        }

        throw std::runtime_error("Enum out of bounds");
    }
}
