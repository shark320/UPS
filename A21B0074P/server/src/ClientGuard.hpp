#ifndef SERVER_CLIENTGUARD_HPP
#define SERVER_CLIENTGUARD_HPP

#include "ClientInfo.hpp"
#include <list>
#include <mutex>
#include <memory>
#include <algorithm>

using std::list;
using std::mutex;
using std::shared_ptr;

class ClientGuard {
private:
    list<shared_ptr<ClientInfo>>& clients;  // List of shared pointers to ClientInfo objects.
    shared_ptr<ClientInfo> client;          // Shared pointer to a ClientInfo object.
    mutex& client_mutex;                    // Reference to a mutex object for synchronization.

public:
    /**
     * @brief Constructor for the ClientGuard class.
     * @param clients A reference to a list of shared pointers to ClientInfo objects.
     * @param client A shared pointer to a ClientInfo object.
     * @param client_mutex A reference to a mutex object for synchronization.
     */
    ClientGuard(list<shared_ptr<ClientInfo>>& clients, shared_ptr<ClientInfo> client, mutex& client_mutex);

    /**
     * @brief Destructor for the ClientGuard class.
     *        It releases the lock on the client_mutex when the object is destroyed.
     */
    ~ClientGuard();
};

#endif //SERVER_CLIENTGUARD_HPP
