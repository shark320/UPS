#ifndef SERVER_LOBBYMANAGER_HPP
#define SERVER_LOBBYMANAGER_HPP

#include "Lobby.hpp"
#include <unordered_map>
#include <string>
#include <mutex>
#include <memory>

using std::unordered_map;
using std::mutex;
using std::string;
using std::shared_ptr;

class ClientInfo;
class ClientManager;

class LobbyManager {
private:
    unordered_map<string, shared_ptr<Lobby>> lobbies;  // Unordered map to store lobbies by name.

public:
    mutex lobby_mutex;  // Mutex for synchronizing access to lobbies.

    /**
     * @brief Default constructor for the LobbyManager class.
     */
    LobbyManager() = default;

    /**
     * @brief Method to create a new lobby.
     * @param opcode The operation code for the action.
     * @param name The name of the new lobby.
     * @param client A shared pointer to the client creating the lobby.
     * @param maxPlayers The maximum number of players allowed in the lobby.
     * @param hasPassword A flag indicating whether the lobby has a password.
     * @param password The password for the lobby (if it has one).
     * @param clientManager A reference to the ClientManager object.
     * @return A string indicating the result of the action.
     */
    string createLobby(const string& opcode, const string& name, shared_ptr<ClientInfo>& client, int maxPlayers, bool hasPassword, const string& password, ClientManager& clientManager);

    /**
     * @brief Method to add a player to an existing lobby.
     * @param opcode The operation code for the action.
     * @param lobbyName The name of the lobby to join.
     * @param client A shared pointer to the client joining the lobby.
     * @param password The password for the lobby (if required).
     * @param clientManager A reference to the ClientManager object.
     * @return A string indicating the result of the action.
     */
    string addPlayerToLobby(const string& opcode, const string& lobbyName, shared_ptr<ClientInfo>& client, const string& password, ClientManager& clientManager);

    /**
     * @brief Method to delete (close) a lobby.
     * @param opcode The operation code for the action.
     * @param lobbyName The name of the lobby to delete.
     * @param client A shared pointer to the client requesting the deletion.
     * @param password The password for the lobby (if required).
     * @param clientManager A reference to the ClientManager object.
     * @return A string indicating the result of the action.
     */
    string deleteLobby(const string& opcode, const string& lobbyName, shared_ptr<ClientInfo>& client, const string& password, ClientManager& clientManager);

    /**
     * @brief Method to convert lobby manager information to a string representation.
     * @param shouldBeLocked A flag indicating whether the lobby manager should be locked.
     * @return A string representation of the lobby manager.
     */
    string toString(bool shouldBeLocked);

    /**
     * @brief Getter method to retrieve the map of lobbies.
     * @return A reference to the unordered map of lobbies.
     */
    unordered_map<string, shared_ptr<Lobby>>& getLobbies();
};

#endif //SERVER_LOBBYMANAGER_HPP