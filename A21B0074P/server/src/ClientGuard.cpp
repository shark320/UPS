#include "ClientGuard.hpp"

using std::lock_guard;
using std::find_if;

ClientGuard::ClientGuard(list<shared_ptr<ClientInfo>>& clients, shared_ptr<ClientInfo> client, mutex& client_mutex)
        : clients(clients), client(std::move(client)), client_mutex(client_mutex) {
}

ClientGuard::~ClientGuard() {
    lock_guard<mutex> lock(client_mutex);
    clients.remove_if([this](const shared_ptr<ClientInfo>& ci) {
        return ci->getClientSocket() == client->getClientSocket();
    });
}
