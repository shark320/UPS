#ifndef SERVER_CONSTANTS_HPP
#define SERVER_CONSTANTS_HPP

#include <string>

using std::string;

extern const int DATALEN;
extern const int HEADER_SIZE;
extern const int OPCODE_OFFSET;
extern const int DATA_LENGTH_OFFSET;
extern const int VALID_HEADER_LENGTH;

extern const bool IS_DEBUG;

extern const char* OPCODE_16;
extern const char* VALID_HEADER;
extern const char* STARTED_RSP;
extern const char* START_CMD;
extern const char* END_CMD;
extern const char* TAKE_CMD;
extern const char* PASS_CMD;
extern const char* DISCONNECT_CMD;
extern const char* DISCONNECT_END_CMD;
extern const char* GAME_LEAVE_CMD;
extern const char* LOGOUT_CMD;
extern const char* PING_CMD;
extern const char* PONG_CMD;
extern const char* EXIT_LOBBY_CMD;
extern const string INVALID_MESSAGE_FORMAT;
extern const string USER_NOT_AUTHENTICATED;
extern const string USER_ALREADY_EXISTS;
extern const string USER_ALREADY_LOGGED_IN_BY_ANOTHER_CLIENT;
extern const string UNKNOWN_COMMAND;
extern const string UNKNOWN_SUBCOMMAND;
extern const string USER_NOT_FOUND;
extern const string INVALID_LOGIN_CREDENTIALS;
extern const string YOU_ARE_ALREADY_LOGGED_IN;
extern const string LOGOUT_SUCCESS;
extern const string NOT_IN_A_LOBBY;
extern const string NO_USER_LOGGED_IN;
extern const string UNKNOWN_OPERATIONAL_CODE;
extern const string GAME_ALREADY_STARTED;
extern const string NOT_ALL_PLAYERS_HAVE_CONNECTED;
extern const string INVALID_MAX_PLAYERS_SHOULD_BE_BETWEEN_1_AND_26;
extern const string ITS_NOT_YOUR_TURN;
extern const string DECK_IS_EMPTY;
extern const string ONLY_THE_LOBBY_ADMIN_CAN_START_A_GAME;
extern const string INCORRECT_PASSWORD;
extern const string LOBBY_NOT_FOUND;
extern const string LOBBY_WITH_THE_SAME_NAME_ALREADY_EXISTS;
extern const string LOBBY_IS_FULL;
extern const string PASSWORD_REQUIRED_TO_DELETE_THIS_LOBBY;
extern const string PASSWORD_REQUIRED_TO_JOIN_THIS_LOBBY;
extern const string YOU_ARE_ALREADY_IN_THIS_LOBBY;
extern const string YOU_ARE_ALREADY_IN_ANOTHER_LOBBY;
extern const string ONLY_THE_LOBBY_CREATOR_CAN_DELETE_THE_LOBBY;
extern const string NO_LOBBIES;
extern const string CANNOT_DELETE_LOBBY_WITH_ACTIVE_GAME;
extern const string CANNOT_ENTER_LOBBY_WITH_ACTIVE_GAME;
extern const string LOBBY_IS_ALREADY_FULL;
extern const string ERROR;
extern const string TURN;

#endif
