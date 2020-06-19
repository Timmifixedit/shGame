//
// Created by tim on 19.06.20.
//

#include "util.h"
#include <iostream>
#include <sstream>

namespace gmUtil {
    std::string promptForInput(const std::string &msg, std::istream &in, std::ostream &out) {
        out << msg << std::endl;
        std::string input;
        std::getline(in, input);
        return input;
    }

    auto splitString(const std::string &string, char delimiter) -> std::vector<std::string> {
        std::vector<std::string> ret;
        std::stringstream tmp(string);
        std::string part;
        while (std::getline(tmp, part, delimiter)) {
            ret.emplace_back(part);
        }

        return ret;
    }

    auto parseRuleType(const std::string &string) -> std::optional<sh::RuleSetType> {
        constexpr auto STANDARD_RULES = "standard";
        if (string == STANDARD_RULES) {
            return sh::RuleSetType::Standard;
        }

        return {};
    }
}
