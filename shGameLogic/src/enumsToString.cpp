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

    auto toCardType(const std::string &string) -> std::optional<sh::CardType> {
        if (string == toString(sh::CardType::Fascist)) {
            return sh::CardType::Fascist;
        } else if (string == toString(sh::CardType::Liberal)) {
            return sh::CardType::Liberal;
        } else {
            return {};
        }
    }

    auto toGovRole(const std::string &string) -> std::optional<sh::Player::GovernmentRole> {
        if (string == toString(sh::Player::GovernmentRole::President)) {
            return sh::Player::GovernmentRole::President;
        } else if (string == toString(sh::Player::GovernmentRole::Chancellor)) {
            return sh::Player::GovernmentRole::Chancellor;
        } else {
            return {};
        }
    }

    auto toPlayerType(const std::string &string) -> std::optional<sh::Player::Type> {
        if (string == toString(sh::Player::Type::Fascist)) {
            return sh::Player::Type::Fascist;
        } else if (string == toString(sh::Player::Type::Liberal)) {
            return sh::Player::Type::Liberal;
        } else if (string == toString(sh::Player::Type::Hitler)) {
            return sh::Player::Type::Hitler;
        } else {
            return {};
        }
    }
}
