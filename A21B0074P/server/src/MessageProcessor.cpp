#include "MessageProcessor.hpp"
#include "ClientInfo.hpp"
#include "ClientManager.hpp"
#include "LobbyManager.hpp"
#include "Constants.hpp"
#include "BlackjackGame.hpp"
#include "Utils.hpp"
#include <vector>
#include <mutex>


using std::unordered_map;
using std::vector;
using std::mutex;
using std::lock_guard;
using std::to_string;

unordered_map<string, MessageProcessor::ProcessorFunc> MessageProcessor::processors = {
        {"00", MessageProcessor::process00}, // registration
        {"01", MessageProcessor::process01}, // login
        {"02", MessageProcessor::process02}, // logout or exit lobby
        {"03", MessageProcessor::process03}, // create lobby
        {"04", MessageProcessor::process04}, // join lobby
        {"05", MessageProcessor::process05}, // delete lobby
        {"06", MessageProcessor::process06}, // game actions
        {"07", MessageProcessor::process07}  // ping
};

string MessageProcessor::process00(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    auto components = splitMessage(message, ';');

    if (components.size() != 4) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto&  login = components[0];
    const auto&  password = components[1];
    const auto&  name = components[2];
    const auto&  surname = components[3];

    if(!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    return clientManager.registerUser(opcode, login, password, name, surname); // returns a response and behaves accordingly
}

string MessageProcessor::process01(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    auto components = splitMessage(message, ';');

    if (components.size() != 2) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& login = components[0];
    const auto& password = components[1];


    if(!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    return clientManager.attemptLogin(opcode, client, login, password, clientManager, lobbyManager); //
}

string MessageProcessor::process02(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!trimCompare(message, LOGOUT_CMD) && !trimCompare(message, EXIT_LOBBY_CMD)) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    return clientManager.processLogoutOrExit(opcode, message, client, clientManager, lobbyManager);
}

string MessageProcessor::process03(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!client->getUser().lock()) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED);
    }

    auto components = splitMessage(message, ';');

    if(!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    if (components.size() < 3 || (components[2] == "1" && components.size() < 4)) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& name = components[0];

    int maxPlayers = 0;
    try {
        maxPlayers = stoi(components[1]);
    } catch (const std::invalid_argument& e) {
        return generateNegativeResponse(opcode, INVALID_MAX_PLAYERS_SHOULD_BE_BETWEEN_1_AND_26);
    } catch (const std::out_of_range& e) {
        return generateNegativeResponse(opcode, INVALID_MAX_PLAYERS_SHOULD_BE_BETWEEN_1_AND_26);
    }

    if (maxPlayers <= 0 || maxPlayers > 26) {
        return generateNegativeResponse(opcode, INVALID_MAX_PLAYERS_SHOULD_BE_BETWEEN_1_AND_26);
    }

    if (!isBinaryString(components[2])) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& hasPassword = components[2] == "1";
    const auto& password = hasPassword ? components[3] : "";

    return lobbyManager.createLobby(opcode, name, client, maxPlayers, hasPassword, password, clientManager);
}

string MessageProcessor::process04(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!client->getUser().lock()) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED);
    }

    auto components = splitMessage(message, ';');

    if (!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    if (components.size() < 2 || (components[1] == "1" && components.size() < 3)) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& lobbyName = components[0];

    if (!isBinaryString(components[1])) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& hasPassword = components[1] == "1";
    const auto& password = hasPassword ? components[2] : "";

    return lobbyManager.addPlayerToLobby(opcode, lobbyName, client, password, clientManager);
}

string MessageProcessor::process05(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!client->getUser().lock()) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED);
    }

    auto components = splitMessage(message, ';');

    if(!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    if (components.size() < 2 || (components[1] == "1" && components.size() < 3)) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    if (!isBinaryString(components[1])) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    const auto& lobbyName = components[0];
    const auto& hasPassword = components[1] == "1";
    const auto& password = hasPassword ? components[2] : "";

    return lobbyManager.deleteLobby(opcode, lobbyName, client, password, clientManager);
}

string MessageProcessor::process06(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!client->getUser().lock()) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED);
    }

    auto lobby_ptr = client->getUser().lock()->getLobby().lock();
    if (!lobby_ptr) {
        return generateNegativeResponse(opcode, NOT_IN_A_LOBBY);
    }

    auto components = splitMessage(message, ';');

    if(!areAllComponentsValid(components)) { // checks if any of the components is empty
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    if (components.size() != 1) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    for (const auto& player : lobby_ptr->getPlayers()) {
        if (!player->getClientInfo().lock()) {
            return generateNegativeResponse(opcode, NOT_ALL_PLAYERS_HAVE_CONNECTED);
        }
    }

    const auto& subcommand = components[0];
    if (trimCompare(subcommand, START_CMD)) {
        if (lobby_ptr->getAdmin() == client->getUser().lock()) {
            if (!lobby_ptr->getGame()) {
                lobby_ptr->setGame(make_shared<BlackjackGame>(lobby_ptr));
                return lobby_ptr->startGame(opcode, clientManager, lobbyManager);
            } else {
                return generateNegativeResponse(opcode, GAME_ALREADY_STARTED);
            }
        } else {
            return generateNegativeResponse(opcode, ONLY_THE_LOBBY_ADMIN_CAN_START_A_GAME);
        }
    } else if (trimCompare(subcommand, TAKE_CMD) || trimCompare(subcommand, PASS_CMD)) {
        auto& game = lobby_ptr->getGame();
        if (!game || lobby_ptr->getGame()->getCurrentPlayer().lock() != client->getUser().lock()) {
            return generateNegativeResponse(opcode, ITS_NOT_YOUR_TURN);
        }

        if (trimCompare(subcommand, TAKE_CMD)) {
            return game->takeAction(opcode, client->getUser().lock());
        } else if (trimCompare(subcommand, PASS_CMD)) {
            return game->passAction(opcode, client->getUser().lock());
        }

        return generateNegativeResponse(opcode, UNKNOWN_SUBCOMMAND);
    } else {
        return generateNegativeResponse(opcode, UNKNOWN_SUBCOMMAND);
    }
}

string MessageProcessor::process07(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!trimCompare(message, PING_CMD)) {
        return generateNegativeResponse(opcode, INVALID_MESSAGE_FORMAT);
    }

    auto user = client->getUser().lock();

    if (user) {
        user->updateLastPingTime();
    }

    return generatePositiveResponse(opcode, PONG_CMD);
}

string MessageProcessor::processMessage(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    auto it = processors.find(opcode);
    if (it != processors.end()) {
        return it->second(opcode, trim(message), client, clientManager, lobbyManager);
    } else {
        return UNKNOWN_OPERATIONAL_CODE;
    }
}