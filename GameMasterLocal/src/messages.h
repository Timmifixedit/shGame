//
// Created by tim on 19.06.20.
//

#ifndef SECRETHITLERGAMEMASTER_MESSAGES_H
#define SECRETHITLERGAMEMASTER_MESSAGES_H

constexpr auto INPUT_PLAYER_NAMES = "Please input the names of all players participating. Input player names separated "
                                    "y whitespaces, confirm with enter";
constexpr auto INPUT_RULE_SET = "Specify the set of rules for your game. Currently only the standard game rules are "
                                "supported, so just specify 'standard'";

constexpr auto INVALID_RULE_SET = "The specified rule set is not known";

constexpr auto EMPTY_NAME = "Player names must not be empty";

#endif //SECRETHITLERGAMEMASTER_MESSAGES_H
