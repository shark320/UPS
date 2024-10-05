#include "Lobby.hpp"
#include "User.hpp"
#include "BlackjackGame.hpp"
#include <memory>
#include <algorithm>
#include <sstream>
#include "Utils.hpp"
#include "Constants.hpp"
#include "ClientManager.hpp"
#include "LobbyManager.hpp"
#include "ClientInfo.hpp"
#include <sys/socket.h>

using std::mutex;
using std::lock_guard;

int Lobby::nextIndex = 0;

Lobby::Lobby()
        : index(nextIndex++), maxPlayers(0), hasPassword(false) {}

Lobby::Lobby(const string& name, int maxPlayers, bool hasPassword, const string& password)
        : index(nextIndex++), name(name), maxPlayers(maxPlayers), hasPassword(hasPassword), password(password) {}

int Lobby::getIndex() const {
    return index;
}

string Lobby::getName() const {
    return name;
}

void Lobby::setName(const string& newName) {
    name = newName;
}

int Lobby::getMaxPlayers() {
    return maxPlayers;
}

const shared_ptr<BlackjackGame> Lobby::getGame() const { return game; }

void Lobby::setGame(const shared_ptr<BlackjackGame>& newGame) { game = newGame; }


bool Lobby::getHasPassword() {
    return hasPassword;
}

string Lobby::getPassword() {
    return password;
}

shared_ptr<User> Lobby::getAdmin() const {
    return admin.lock();
}

void Lobby::setAdmin(const shared_ptr<User>& newAdmin) {
    admin = newAdmin;
}

shared_ptr<User> Lobby::getCreator() const {
    return creator.lock();
}

void Lobby::setCreator(const shared_ptr<User>& newCreator) {
    creator = newCreator;
}

list<shared_ptr<User>> Lobby::getPlayers() {
    std::lock_guard<std::mutex> lock(game_mutex);
    list<shared_ptr<User>> activePlayers;
    for (auto& weakPlayer : players) {
        if (auto player = weakPlayer.lock()) {
            activePlayers.push_back(player);
        }
    }
    return activePlayers;
}

string Lobby::startGame(const string& opcode, ClientManager& clientManager, LobbyManager& lobbyManager) {
    if (!game) {
        game = std::make_shared<BlackjackGame>(shared_from_this());
    }

    return game->startGame(opcode, getPlayers(), clientManager, lobbyManager);
}

string Lobby::finishGame() {
    std::lock_guard<std::mutex> lock(game_mutex);
    if (game) {
        game->endGame();
        game.reset();
    }

    return "";
}

string Lobby::addPlayer(const string& opcode, const shared_ptr<User>& user) {
    lock_guard<mutex> lock(game_mutex);
    auto userInLobby = std::find_if(players.begin(), players.end(),
                                    [&user](const weak_ptr<User>& existingUser) {
                                        return existingUser.lock() == user;
                                    });
    if (userInLobby != players.end()) {
        return generateNegativeResponse(opcode, YOU_ARE_ALREADY_IN_ANOTHER_LOBBY);
    }

    if (players.size() < maxPlayers) {
        players.push_back(user);
        user->setLobby(shared_from_this());

        if (!getAdmin()) {
            setAdmin(user);
        }

        return generatePositiveResponse(opcode, toString(false, user));
    } else {
        return generateNegativeResponse(opcode, LOBBY_IS_ALREADY_FULL);
    }
}


void Lobby::removePlayer(const shared_ptr<User>& user) {
    std::lock_guard<std::mutex> lock(game_mutex);
    auto it = std::find_if(players.begin(), players.end(),
                           [&user](const weak_ptr<User>& p) { return p.lock() == user; });
    if (it != players.end()) {
        players.erase(it);
        user->setLobby(nullptr);
    }
}

string Lobby::toString(bool shouldBeLocked, shared_ptr<User> player) {
    std::stringstream ss;
    if (shouldBeLocked) {
        std::lock_guard<std::mutex> lock(game_mutex);
    }

    ss << (game ? "GAME[" : "LOBBY[") << index << ";" << name << ";" << maxPlayers << ";" << (hasPassword ? "1" : "0") << ";" << players.size() << ";";

    auto adminPtr = admin.lock();
    ss << (adminPtr ? adminPtr->toString() : "[]") << ";";

    auto creatorPtr = creator.lock();
    ss << (creatorPtr ? creatorPtr->toString() : "[]") << ";";

    ss << "[";
    for (const auto& weakPlayer : players) {
        auto playerPtr = weakPlayer.lock();
        if (playerPtr) {
            ss << playerPtr->toString() << ";";
        }
    }

    if (!players.empty()) {
        ss.seekp(-1, std::ios_base::end);
    }

    ss << "];";

    if (auto gamePtr = game) {
        ss << "1;" << gamePtr->toString(player);
    } else {
        ss << "0";
    }

    ss << "]";
    return ss.str();
}

string Lobby::toLobbyManagerString() {
    std::stringstream ss;
    std::lock_guard<std::mutex> lock(game_mutex);

    ss << "[" << index << ";" << name << ";" << maxPlayers << ";" << (hasPassword ? "1" : "0") << ";" << players.size() << ";";

    auto adminPtr = admin.lock();
    ss << (adminPtr ? adminPtr->toString() : "[]") << ";";

    auto creatorPtr = creator.lock();
    ss << (creatorPtr ? creatorPtr->toString() : "[]") << ";";

    if (auto gamePtr = game) {
        ss << "1";
    } else {
        ss << "0";
    }

    ss << "]";
    return ss.str();
}
