#ifndef SERVER_USER_HPP
#define SERVER_USER_HPP

#include "Lobby.hpp"
#include <string>
#include <memory>

using std::string;
using std::weak_ptr;
using std::shared_ptr;

class ClientInfo;
class LobbyManager;
class ClientManager;

class User {
private:
    string login;  // User's login.
    string password;  // User's password.
    string name;  // User's name.
    string surname;  // User's surname.
    int disconnectCounter;  // Counter to keep track of disconnects.
    weak_ptr<Lobby> lobby;  // Weak pointer to the lobby the user is in (if any).
    weak_ptr<ClientInfo> clientInfo;  // Weak pointer to the client information associated with the user.
    std::chrono::steady_clock::time_point lastDisconnectIncrement;  // Timestamp of the last disconnect increment.
    std::chrono::steady_clock::time_point lastPingTime = std::chrono::steady_clock::now();  // Timestamp of the last ping time.

public:
    /**
     * @brief Default constructor for the User class.
     */
    User();

    /**
     * @brief Constructor for the User class with parameters.
     * @param login The user's login.
     * @param password The user's password.
     * @param name The user's name.
     * @param surname The user's surname.
     * @param disconnectCounter The initial disconnect counter value (default is 0).
     */
    User(const string& login, const string& password, const string& name, const string& surname, int disconnectCounter = 0);

    /**
     * @brief Getter method to retrieve the user's login.
     * @return The user's login.
     */
    const string& getLogin();

    /**
     * @brief Setter method to update the user's login.
     * @param newLogin The new login to set.
     */
    void setLogin(const string& newLogin);

    /**
     * @brief Setter method to update the user's password.
     * @param newPassword The new password to set.
     */
    void setPassword(const string& newPassword);

    /**
     * @brief Getter method to retrieve the user's password.
     * @return The user's password.
     */
    string getPassword() const;

    /**
     * @brief Getter method to retrieve the user's name.
     * @return The user's name.
     */
    const string& getName();

    /**
     * @brief Setter method to update the user's name.
     * @param newName The new name to set.
     */
    void setName(const string& newName);

    /**
     * @brief Getter method to retrieve the user's surname.
     * @return The user's surname.
     */
    const string& getSurname();

    /**
     * @brief Setter method to update the user's surname.
     * @param newSurname The new surname to set.
     */
    void setSurname(const string& newSurname);

    /**
     * @brief Getter method to retrieve the lobby the user is in (if any).
     * @return A weak pointer to the lobby the user is in.
     */
    weak_ptr<Lobby> getLobby();

    /**
     * @brief Setter method to update the lobby the user is in.
     * @param newLobby A shared pointer to the new lobby to set.
     */
    void setLobby(const shared_ptr<Lobby>& newLobby);

    /**
     * @brief Setter method to update the client information associated with the user.
     * @param newClientInfo A weak pointer to the new client information to set.
     */
    void setClientInfo(const weak_ptr<ClientInfo>& newClientInfo);

    /**
     * @brief Getter method to retrieve the client information associated with the user.
     * @return A weak pointer to the client information associated with the user.
     */
    weak_ptr<ClientInfo> getClientInfo() const;

    /**
     * @brief Method to retrieve lobby information for the user.
     * @return A string representation of the user's lobby information.
     */
    string getLobbyInfo() const;

    /**
     * @brief Getter method to retrieve the disconnect counter value.
     * @return The disconnect counter value.
     */
    int getDisconnectCounter() const;

    /**
     * @brief Setter method to update the disconnect counter value.
     * @param disconnectCounter The new disconnect counter value to set.
     */
    void setDisconnectCounter(int disconnectCounter);

    /**
     * @brief Method to increment the disconnect counter.
     */
    void incrementDisconnectCounter();

    /**
     * @brief Method to convert user information to a string representation.
     * @return A string representation of the user.
     */
    string toString();

    /**
     * @brief Method to convert user information to a string representation including the password.
     * @return A string representation of the user including the password.
     */
    string toStringWithPassword();

    /**
     * @brief Method to retrieve the login state of the user.
     * @param clientManager A reference to the ClientManager object.
     * @param lobbyManager A reference to the LobbyManager object.
     * @param user A weak pointer to the user for whom the login state is retrieved.
     * @return A string representing the login state of the user.
     */
    string getLoginState(ClientManager& clientManager, LobbyManager& lobbyManager, weak_ptr<User> user);

    /**
     * @brief Method to update the timestamp of the last ping time.
     */
    void updateLastPingTime();

    /**
     * @brief Method to check if a ping timeout has occurred.
     * @return True if a ping timeout has occurred, false otherwise.
     */
    bool isPingTimeout() const;
};

#endif //SERVER_USER_HPP
