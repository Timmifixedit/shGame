//
// Created by tim on 19.06.20.
//

#ifndef SECRETHITLERGAMEMASTER_UTIL_H
#define SECRETHITLERGAMEMASTER_UTIL_H

#include <string>
#include <vector>
#include <optional>
#include <SecretHitlerGameLogic/rules.h>
#include <SecretHitlerGameLogic/shGameLogic.h>

namespace gmUtil {
    enum class GameState {
        ElectChancellor, DiscardFirstPolicy, ChooseFinalPolicy, HandlePolicyEvent, InitGame
    };

    /**
     * Prompts the user for an input additionally displaying a message
     * @param msg message to display the user
     * @param in input stream to read the user data from
     * @param out output stream to report to the user
     * @return user input
     */
    std::string promptForInput(const std::string &msg, std::istream &in, std::ostream &out);

    /**
     * Splits a string using the specified delimiter
     * @param string string to process
     * @param delimiter delimiter character
     * @return vector of strings. May include empty strings
     */
    auto splitString(const std::string &string, char delimiter) -> std::vector<std::string>;

    /**
     * Checks the input against all implemented rule sets of the game
     * @param string
     * @return Corresponding rule type or nothing if no match was found
     */
    auto parseRuleType(const std::string &string) -> std::optional<sh::RuleSetType>;

    /**
     * Asks the user for confirmation
     * @param msg message to print to the user
     * @param in input stream to read the user data from
     * @param out output stream to report to the user
     * @return true if confirmation successful, false otherwise
     */
    bool getConfirmation(const std::string &msg, std::istream &in, std::ostream &out);

    void printGameStatus(std::ostream &out, const sh::Game &game);

    std::string promptForPlayer(std::istream &in, std::ostream &out, const sh::Game &game);

    std::string promptForPlayerAndConfirm(std::istream &in, std::ostream &out, const sh::Game &game);

    sh::CardType promptPlayerForCard(std::istream &in, std::ostream &out, const sh::CardRange &cards);
}


#endif //SECRETHITLERGAMEMASTER_UTIL_H
