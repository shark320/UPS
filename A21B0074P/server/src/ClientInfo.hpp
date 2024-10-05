#ifndef SERVER_CLIENTINFO_HPP
#define SERVER_CLIENTINFO_HPP

#include <string>
#include <memory>
#include <chrono>

using std::string;
using std::weak_ptr;
using std::shared_ptr;
using std::chrono::steady_clock;

class User;

class ClientInfo {
private:
    int client_socket;         // Integer representing the client's socket.
    string addr;               // String representing the client's address.
    weak_ptr<User> user;       // Weak pointer to a User object associated with the client.

public:
    /**
     * @brief Constructor for the ClientInfo class.
     * @param socket The client's socket identifier.
     * @param address The client's address as a string.
     */
    ClientInfo(int socket, const string& address);

    /**
     * @brief Getter method to retrieve the client's socket identifier.
     * @return The client's socket identifier.
     */
    int getClientSocket() const;

    /**
     * @brief Setter method to update the client's socket identifier.
     * @param newSocket The new socket identifier to set.
     */
    void setClientSocket(int newSocket);

    /**
     * @brief Getter method to retrieve the client's address.
     * @return A constant reference to the client's address as a string.
     */
    const string& getAddr() const;

    /**
     * @brief Setter method to associate a User object with the client.
     * @param newUser A shared pointer to the User object to associate.
     */
    void setUser(shared_ptr<User> newUser);

    /**
     * @brief Getter method to retrieve the associated User object as a weak pointer.
     * @return A weak pointer to the associated User object.
     */
    weak_ptr<User> getUser() const;
};

#endif //SERVER_CLIENTINFO_HPP
