#include "LobbyManager.hpp"
#include "User.hpp"
#include "Utils.hpp"
#include "ClientInfo.hpp"
#include <sys/socket.h>
#include "Constants.hpp"
#include "ClientManager.hpp"


using std::lock_guard;
using std::lock;
using std::to_string;

string LobbyManager::createLobby(const string& opcode, const string& name, shared_ptr<ClientInfo>& client, int maxPlayers, bool hasPassword, const string& password, ClientManager& clientManager) {
    lock_guard<mutex> lock(lobby_mutex);
    if (lobbies.find(name) != lobbies.end()) {
        return generateNegativeResponse(opcode, LOBBY_WITH_THE_SAME_NAME_ALREADY_EXISTS);
    }

    shared_ptr<Lobby> newLobby = std::make_shared<Lobby>(name, maxPlayers, hasPassword, password);
    lobbies[name] = newLobby;
    newLobby ->setCreator(client->getUser().lock());

    string notification = generatePositiveResponse(opcode, toString(false));

    for (const auto& userPair : clientManager.getUsers()) {
        auto user = userPair.second;
        if (client->getUser().lock() != user) {
            if (!user->getLobby().lock()) {
                if (user->getClientInfo().lock()) {
                    send(user->getClientInfo().lock()->getClientSocket(), notification.c_str(), notification.size(), MSG_NOSIGNAL);
                }
            }
        }
    }

    return notification;
}

unordered_map<string, shared_ptr<Lobby>>& LobbyManager::getLobbies() {
    return lobbies;
}

string LobbyManager::addPlayerToLobby(const string& opcode, const string& lobbyName, shared_ptr<ClientInfo>& client, const string& password, ClientManager& clientManager) {
    lock_guard<mutex> lock(lobby_mutex);
    auto it = lobbies.find(lobbyName);
    if (it == lobbies.end()) {
        return generateNegativeResponse(opcode, LOBBY_NOT_FOUND);
    }

    auto& lobby = it->second;
    auto user = client->getUser().lock();
    if (!user) {
        return generateNegativeResponse(opcode, USER_NOT_AUTHENTICATED);
    }

    if (lobby->getGame()) {
        return generateNegativeResponse(opcode, CANNOT_ENTER_LOBBY_WITH_ACTIVE_GAME);
    }

    if (auto currentLobby = user->getLobby().lock()) {
        if (currentLobby == lobby) {
            return generateNegativeResponse(opcode, YOU_ARE_ALREADY_IN_THIS_LOBBY);
        } else {
            return generateNegativeResponse(opcode, YOU_ARE_ALREADY_IN_ANOTHER_LOBBY);
        }
    }

    if (static_cast<int>(lobby->getPlayers().size()) >= lobby->getMaxPlayers()) {
        return generateNegativeResponse(opcode, LOBBY_IS_FULL);
    }

    if (lobby->getHasPassword() && password.empty()) {
        return generateNegativeResponse(opcode, PASSWORD_REQUIRED_TO_JOIN_THIS_LOBBY);
    }

    if (lobby->getHasPassword() && !trimCompare(lobby->getPassword(), password)) {
        return generateNegativeResponse(opcode, INCORRECT_PASSWORD);
    }

    string response = lobby->addPlayer(opcode, user);

    string notificationNotInLobby = generatePositiveResponse(opcode, toString(false));
    string notificationInLobby = generatePositiveResponse(opcode, lobby->toString(true, user));

    for (const auto& userPair : clientManager.getUsers()) {
        auto userAut = userPair.second;
        if (userAut->getClientInfo().lock()) {
            if (!userAut->getLobby().lock()) {
                if (userAut->getClientInfo().lock() && userAut != user) {
                    send(userAut->getClientInfo().lock()->getClientSocket(), notificationNotInLobby.c_str(),
                         notificationNotInLobby.size(), MSG_NOSIGNAL);
                }
            } else if (userAut->getLobby().lock() == lobby && userAut != user) {
                send(userAut->getClientInfo().lock()->getClientSocket(), notificationInLobby.c_str(),
                     notificationInLobby.size(), MSG_NOSIGNAL);
            }
        }
    }

    return response;
}

string LobbyManager::deleteLobby(const string& opcode, const string& lobbyName, shared_ptr<ClientInfo>& client, const string& password, ClientManager& clientManager) {
    lock_guard<mutex> lock(lobby_mutex);
    auto it = lobbies.find(lobbyName);
    if (it == lobbies.end()) {
        return generateNegativeResponse(opcode, LOBBY_NOT_FOUND);
    }

    auto& lobby = it->second;
    if (lobby->getCreator() != client->getUser().lock()) {
        return generateNegativeResponse(opcode, ONLY_THE_LOBBY_CREATOR_CAN_DELETE_THE_LOBBY);
    }

    if (lobby->getHasPassword() && password.empty()) {
        return generateNegativeResponse(opcode, PASSWORD_REQUIRED_TO_DELETE_THIS_LOBBY);
    }

    if (lobby->getHasPassword() && !trimCompare(lobby->getPassword(), password)) {
        return generateNegativeResponse(opcode, INCORRECT_PASSWORD);
    }

    if (lobby->getGame()) {
        return generateNegativeResponse(opcode, CANNOT_DELETE_LOBBY_WITH_ACTIVE_GAME);
    }

    for (auto player : lobby->getPlayers()) {
        player->setLobby(nullptr);
    }

    lobbies.erase(it);

    string notification = generatePositiveResponse(opcode, toString(false));

    for (const auto& userPair : clientManager.getUsers()) {
        auto user = userPair.second;
        if (client->getUser().lock() != user) {
            if (!user->getLobby().lock()) {
                if (user->getClientInfo().lock()) {
                    send(user->getClientInfo().lock()->getClientSocket(), notification.c_str(), notification.size(), MSG_NOSIGNAL);
                }
            }
        }
    }

    return notification;
}

string LobbyManager::toString(bool shouldBeLocked) {
    if (shouldBeLocked) {
        lock_guard<mutex> lock(lobby_mutex);
    }
    string result;

    if (!lobbies.empty()) {
        result += "MENU[";

        for (const auto& pair : lobbies) {
            result += pair.second->toLobbyManagerString() + ";";
        }

        result.pop_back();

        result += "]";
    }

    return result.empty() ? NO_LOBBIES : result;
}
