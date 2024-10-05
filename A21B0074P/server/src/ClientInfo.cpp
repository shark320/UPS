#include "ClientInfo.hpp"
#include "User.hpp"

ClientInfo::ClientInfo(int socket, const string& address)
        : client_socket(socket), addr(address) {}

int ClientInfo::getClientSocket() const {
    return client_socket;
}

void ClientInfo::setClientSocket(int newSocket) {
    client_socket = newSocket;
}

const string& ClientInfo::getAddr() const {
    return addr;
}

void ClientInfo::setUser(shared_ptr<User> newUser) {
    user = move(newUser);
}

weak_ptr<User> ClientInfo::getUser() const {
    return user;
}