//
// Created by tim on 19.06.20.
//

#ifndef SECRETHITLERGAMEMASTER_MESSAGES_H
#define SECRETHITLERGAMEMASTER_MESSAGES_H

#include <string>

namespace messages {
    constexpr auto INPUT_PLAYER_NAMES = "Please input the names of all players participating. Input player names separated "
                                        "whitespaces, confirm with enter";
    constexpr auto INPUT_RULE_SET = "Specify the set of rules for your game. Currently only the standard game rules are "
                                    "supported, so just specify 'standard'";
    constexpr auto INVALID_RULE_SET = "The specified rule set is not known";
    constexpr auto DUPLICATE_NAME = "Player names must be unique";
    constexpr auto ELECT_CHANCELLOR = "Elect a new chancellor. If the election was unsuccessful, type nothing";
    constexpr auto PLAYER_NOT_FOUND = "Player name is not known";
    constexpr auto PLAYER_INELIGIBLE = "Cannot elect player '%s'. They were in the last administration";
    constexpr auto PLAYER_DEAD = "This player is dead";
    constexpr auto ELECTION_SUCCESS = "Election successful";
    constexpr auto PRES_DISCARD_CARD = "The president (%s) will now choose one of the following cards that will be discarded";
    constexpr auto CHOOSE_CARD = "Chose a card";
    constexpr auto INVALID_CARD_TYPE = "%s is no appropriate policy type";
    constexpr auto PLAY_POLICY = "The chancellor (%s) will now choose one of the following cards to play as policy";
    constexpr auto INVALID_POLICY = "You cannot choose a %s policy";
    constexpr auto CONFIRM_DECISION = "Please confirm your decision by typing yes. Abort by typing anything else";
    constexpr auto CONFIRM_ELECTION_FAILED = "Confirm that the election failed and that no government will be elected.";
    constexpr auto PLAYER_IN_LAST_GOV = "last administration";
    constexpr auto PLAYER_IS_CANDIDATE = "candidate (%s)";
    constexpr auto PLAYER_IN_GOV = "current government (%d)";
    constexpr auto PLAYER_ORDER = "This is the seating order (circular from top to bottom)";
    constexpr auto LIBERALS_WON_CARDS = "The liberals won by enacting %s liberal policies!";
    constexpr auto LIBERALS_WON_KILL = "The liberals won by killing Hitler!";
    constexpr auto FASCISTS_WON_CARDS = "The fascists won by enacting %s fascist policies!";
    constexpr auto FASCISTS_WON_HITLER = "The fascists won by setting up Hitler as Chancellor!";
    constexpr auto PRES_INVESTIGATE = "The president (%s) will now check the loyalty of a player of their choosing";
    constexpr auto CHOOSE_PLAYER = "Choose a player";
    constexpr auto PRES_EXECUTION = "The president (%s) will now choose a player to be executed";
    constexpr auto PLAYER_EXECUTED = "%s has been executed";
    constexpr auto POLICY_BOARD = "This is the current state of the policy board";
    constexpr auto PRES_SPECIAL_ELECTION = "The president (%s) will now choose a player to be the next president";
    constexpr auto VETO_HINT = "The chancellor has the right to veto this agenda. To do so input 'Veto'";
    constexpr auto VETO_USED = "The current administration used the veto power. No policy was played";
    constexpr auto RANDOM_POLICY = "The public is upset about the government doing nothing so they decided to take "
                                   "things into their own hands. A %s policy was randomly enacted";
    constexpr auto ELECTION_TRACKER = "The election tracker is at %d";
    constexpr auto NUM_CARDS_LEFT = "There are %d cards left on the card pile";
    constexpr auto WELCOME = "Welcome to Secret Hitler. All players have been assigned their role. They are as followed";
    constexpr auto ROUND_OVER = "--- Round over ---";
    constexpr auto NEW_ROUND = "--- A new round has started. This is round %d ---";
    constexpr auto ELECTION_PHASE = "--- Election phase ---";
    constexpr auto EXECUTIVE_PHASE = "--- Executive phase ---";
    constexpr auto EVENT_PHASE = "--- Event phase ---";
}


#endif //SECRETHITLERGAMEMASTER_MESSAGES_H
