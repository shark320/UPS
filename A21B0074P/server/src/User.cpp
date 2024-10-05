#include "User.hpp"
#include "ClientInfo.hpp"
#include <sstream>
#include "LobbyManager.hpp"
#include "ClientManager.hpp"

using std::stringstream;

User::User() : login(""), password(""), name(""), surname("") {}

User::User(const string& login, const string& password, const string& name, const string& surname, int disconnectCounter)
        : login(login), password(password), name(name), surname(surname) {}

const string& User::getLogin() {
    return login;
}

void User::setLogin(const string& newLogin) {
    login = newLogin;
}

void User::setPassword(const string& newPassword) {
    password = newPassword;
}

const string& User::getName() {
    return name;
}

string User::getPassword() const {
    return password;
}

void User::setName(const string& newName) {
    name = newName;
}

const string& User::getSurname() {
    return surname;
}

void User::setSurname(const string& newSurname) {
    surname = newSurname;
}

weak_ptr<Lobby> User::getLobby() {
    return lobby;
}

void User::setLobby(const std::shared_ptr<Lobby>& newLobby) {
    lobby = newLobby;
}

void User::setClientInfo(const weak_ptr<ClientInfo>& newClientInfo) {
    clientInfo = newClientInfo;
}

weak_ptr<ClientInfo> User::getClientInfo() const {
    return clientInfo;
}

string User::toString() {
        std::stringstream ss;

        ss << "[" << login <<";" << name << ";" << surname << ";" << (clientInfo.lock() ? "1" : "0") << "]";

        return ss.str();
}

string User::toStringWithPassword() {
    std::stringstream ss;

    ss << "[" << login <<";" << password << ";" << name << ";" << surname << "]";

    return ss.str();
}

string User::getLoginState(ClientManager& clientManager, LobbyManager& lobbyManager, weak_ptr<User> user) {
    std::stringstream ss;

    ss << "[" << login <<";" << name << ";" << surname << ";" << (clientInfo.lock() ? "1" : "0") << "];";
    if (!getLobby().lock()) {
        ss << lobbyManager.toString(true);
    } else if (getLobby().lock()) {
        ss << getLobby().lock()->toString(true, user.lock());
    }

    return ss.str();
}

int User::getDisconnectCounter() const {
    return disconnectCounter;
}

void User::setDisconnectCounter(int disconnectCounter) {
    User::disconnectCounter = disconnectCounter;
}

void User::incrementDisconnectCounter() {
    auto now = std::chrono::steady_clock::now();
    if (now - lastDisconnectIncrement > std::chrono::seconds(5)) {
        disconnectCounter++;
        lastDisconnectIncrement = now;
    }
}

void User::updateLastPingTime() {
    lastPingTime = std::chrono::steady_clock::now();
}

bool User::isPingTimeout() const {
    return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - lastPingTime).count() > 10;
}