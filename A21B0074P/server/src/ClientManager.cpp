#include "ClientManager.hpp"
#include "ClientInfo.hpp"
#include <mutex>
#include <netinet/in.h>
#include "User.hpp"
#include <list>
#include <unordered_map>
#include <string>
#include <Utils.hpp>
#include <Constants.hpp>
#include <BlackjackGame.hpp>

using std::lock_guard;
using std::string;
using std::mutex;
using std::make_pair;


string ClientManager::registerUser(const string& opcode, const string& login, const string& password, const string& name, const string& surname) {
    lock_guard<mutex> lock(client_mutex);
    auto result = users.emplace(make_pair(login, make_shared<User>(trimEndOfLine(login), trimEndOfLine(password), trimEndOfLine(name), trimEndOfLine(surname))));

    return !result.second ? generateNegativeResponse(opcode, USER_ALREADY_EXISTS) : generatePositiveResponse(opcode, result.first->second->toStringWithPassword());
}

shared_ptr<ClientInfo> ClientManager::addClient(int socket, const string& address) {
    lock_guard<mutex> lock(client_mutex);
    clients.push_back(std::make_shared<ClientInfo>(socket, address));
    return clients.back();
}

string ClientManager::attemptLogin(const string& opcode, shared_ptr<ClientInfo>& client, const string& login, const string& password, ClientManager& clientManager, LobbyManager& lobbyManager) {
    lock_guard<mutex> lock(client_mutex);

    if (client->getUser().lock()) {
        return generateNegativeResponse(opcode, YOU_ARE_ALREADY_LOGGED_IN); // you are already logged in
    }

    for (const auto& other_client : clients) {
        if (other_client->getUser().lock() && other_client->getUser().lock()->getLogin() == login) {
            return generateNegativeResponse(opcode, USER_ALREADY_LOGGED_IN_BY_ANOTHER_CLIENT); // user is already logged in
        }
    }

    auto it = users.find(login);
    if (it == users.end()) {
        return generateNegativeResponse(opcode, USER_NOT_FOUND); // user not found
    }

    if (trimCompare(password, it->second->getPassword())) { // successful login
        client->setUser(it->second);
        it->second->setClientInfo(client);
        auto user = client->getUser().lock();
        user->setDisconnectCounter(0); // reset disconnect counter on successful login
        user->updateLastPingTime(); // update last ping time on successful login
        auto lobby_ptr = user->getLobby().lock();
        if (lobby_ptr) {
            informLobbyMenuAndLobby(opcode, client, clientManager, lobbyManager, lobby_ptr); // inform lobby menu and lobby about the new user in the lobby
        }
        return generatePositiveResponse(opcode, client->getUser().lock()->getLoginState(clientManager, lobbyManager, client->getUser()));
    } else {
        return generateNegativeResponse(opcode, INVALID_LOGIN_CREDENTIALS);
    }
}

list<shared_ptr<ClientInfo>>& ClientManager::getClients() {
    return clients;
}

const unordered_map<string, shared_ptr<User>>& ClientManager::getUsers() const {
    return users;
}

string ClientManager::logout(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    lock_guard<mutex> lock(client_mutex);
    if (auto user = client->getUser().lock()) {
        user->setClientInfo(weak_ptr<ClientInfo>()); // reset client info
        client->setUser(nullptr); // reset user
        return generatePositiveResponse(opcode, LOGOUT_SUCCESS);
    } else {
        return generateNegativeResponse(opcode, NO_USER_LOGGED_IN);
    }
}

string ClientManager::exitLobby(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    lock_guard<mutex> lock(client_mutex);
    if (!client->getUser().lock()) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED); // user not authenticated
    }

    auto lobby_ptr = client->getUser().lock()->getLobby().lock();
    if (!lobby_ptr) {
        return generateNegativeResponse(opcode, NOT_IN_A_LOBBY); // not in a lobby
    }

    for (auto& recipient : lobby_ptr->getPlayers()) { // inform other players in the lobby about the user leaving the lobby
        if (lobby_ptr->getGame() != nullptr) {
            if (recipient == client->getUser().lock()) {
                continue;
            } else {
                string gameLeaveStr = lobby_ptr->getGame()->generateGameDisconnectResponse(client->getUser().lock(), GAME_LEAVE_CMD);
                lobby_ptr->getGame()->sendResponseToPlayer(gameLeaveStr, recipient);
            }
        }
    }

    auto isAdmin = lobby_ptr->getAdmin() == client->getUser().lock();

    lobby_ptr->removePlayer(client->getUser().lock()); // remove the user from the lobby
    if (isAdmin) {
        if (lobby_ptr->getPlayers().empty()) { // change the admin of the lobby if the lobby is not empty
            lobby_ptr->setAdmin(nullptr);
        } else {
            lobby_ptr->setAdmin(lobby_ptr->getPlayers().front());
        }
    }

    client->getUser().lock()->getLobby().reset(); // reset lobby of the user

    lobby_ptr->finishGame(); // finish the game if it is in progress

    informLobbyMenuAndLobby(opcode, client, clientManager, lobbyManager, lobby_ptr); // inform lobby menu and lobby about the user leaving the lobby

    return generatePositiveResponse(opcode, lobbyManager.toString(true));
}

void ClientManager::informLobbyMenuAndLobby(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager, shared_ptr<Lobby>& lobby_ptr) {
    string notificationNotInLobby = generatePositiveResponse(opcode, lobbyManager.toString(true));
    string notificationInLobby = generatePositiveResponse(opcode, lobby_ptr->toString(true, client->getUser().lock()));

    for (const auto& userPair : clientManager.getUsers()) {
        auto userAut = userPair.second;
        if (userAut->getClientInfo().lock()) {
            if (!userAut->getLobby().lock() && userAut != client->getUser().lock()) {
                if (userAut->getClientInfo().lock()) {
                    send(userAut->getClientInfo().lock()->getClientSocket(), notificationNotInLobby.c_str(),
                         notificationNotInLobby.size(), MSG_NOSIGNAL);
                }
            } else if (userAut->getLobby().lock() == lobby_ptr && userAut != client->getUser().lock() && userAut->getClientInfo().lock()) {
                send(userAut->getClientInfo().lock()->getClientSocket(), notificationInLobby.c_str(),
                     notificationInLobby.size(), MSG_NOSIGNAL);
            }
        }
    }
}

void ClientManager::informLobbyMenuAndLobby(const string& opcode, shared_ptr<User>& user, ClientManager& clientManager, LobbyManager& lobbyManager, shared_ptr<Lobby>& lobby_ptr) {
    string notificationNotInLobby = generatePositiveResponse(opcode, lobbyManager.toString(true));
    string notificationInLobby = generatePositiveResponse(opcode, lobby_ptr->toString(true, user));

    for (const auto& userPair : clientManager.getUsers()) {
        auto userAut = userPair.second;
        if (userAut->getClientInfo().lock()) {
            if (!userAut->getLobby().lock() && userAut != user) {
                if (userAut->getClientInfo().lock()) {
                    send(userAut->getClientInfo().lock()->getClientSocket(), notificationNotInLobby.c_str(),
                         notificationNotInLobby.size(), MSG_NOSIGNAL);
                }
            } else if (userAut->getLobby().lock() == lobby_ptr && userAut != user && userAut->getClientInfo().lock()) {
                send(userAut->getClientInfo().lock()->getClientSocket(), notificationInLobby.c_str(),
                     notificationInLobby.size(), MSG_NOSIGNAL);
            }
        }
    }
}

string ClientManager::processLogoutOrExit(const string& opcode, const string& command, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (trimCompare(command, LOGOUT_CMD)) {
        return logout(opcode, client, clientManager, lobbyManager);
    } else if (trimCompare(command, EXIT_LOBBY_CMD)) {
        return exitLobby(opcode, client, clientManager, lobbyManager);
    } else {
        return generateNegativeResponse(opcode, UNKNOWN_COMMAND);
    }
}

