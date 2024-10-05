#ifndef SERVER_LOBBY_HPP
#define SERVER_LOBBY_HPP

#include <list>
#include <mutex>
#include <string>
#include <memory>

// Forward declaration
class BlackjackGame;
class ClientManager;
class LobbyManager;
class User;

using std::string;
using std::list;
using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

class Lobby : public std::enable_shared_from_this<Lobby> {
private:
    static int nextIndex;
    int index;
    string name;
    int maxPlayers;
    bool hasPassword;
    string password;
    weak_ptr<User> admin;
    weak_ptr<User> creator;
    list<weak_ptr<User>> players;
    shared_ptr<BlackjackGame> game;
    std::mutex game_mutex;

public:
    /**
     * @brief Default constructor for the Lobby class.
     */
    Lobby();

    /**
     * @brief Constructor for the Lobby class with parameters.
     * @param name The name of the lobby.
     * @param maxPlayers The maximum number of players allowed in the lobby.
     * @param hasPassword A boolean indicating whether the lobby has a password.
     * @param password The password for the lobby (if it has one).
     */
    Lobby(const string& name, int maxPlayers, bool hasPassword, const string& password);

    /**
     * @brief Getter method to retrieve the index of the lobby.
     * @return The index of the lobby.
     */
    int getIndex() const;

    /**
     * @brief Getter method to retrieve the name of the lobby.
     * @return The name of the lobby.
     */
    string getName() const;

    /**
     * @brief Setter method to update the name of the lobby.
     * @param name The new name for the lobby.
     */
    void setName(const string& name);

    /**
     * @brief Getter method to retrieve the maximum number of players allowed in the lobby.
     * @return The maximum number of players allowed in the lobby.
     */
    int getMaxPlayers();

    /**
     * @brief Getter method to check if the lobby has a password.
     * @return True if the lobby has a password, false otherwise.
     */
    bool getHasPassword();

    /**
     * @brief Getter method to retrieve the password of the lobby (if it has one).
     * @return The password of the lobby.
     */
    string getPassword();

    /**
     * @brief Getter method to retrieve the admin (administrator) of the lobby.
     * @return A shared pointer to the admin user.
     */
    shared_ptr<User> getAdmin() const;

    /**
     * @brief Setter method to update the admin (administrator) of the lobby.
     * @param newAdmin A shared pointer to the new admin user.
     */
    void setAdmin(const shared_ptr<User>& newAdmin);

    /**
     * @brief Getter method to retrieve the creator of the lobby.
     * @return A shared pointer to the creator user.
     */
    shared_ptr<User> getCreator() const;

    /**
     * @brief Setter method to update the creator of the lobby.
     * @param newCreator A shared pointer to the new creator user.
     */
    void setCreator(const shared_ptr<User>& newCreator);

    /**
     * @brief Getter method to retrieve the list of players in the lobby.
     * @return A list of shared pointers to player users.
     */
    list<shared_ptr<User>> getPlayers();

    /**
     * @brief Method to add a player to the lobby.
     * @param opcode The operation code for the action.
     * @param user A shared pointer to the user to be added.
     * @return A string indicating the result of the action.
     */
    string addPlayer(const string& opcode, const shared_ptr<User>& user);

    /**
     * @brief Method to remove a player from the lobby.
     * @param user A shared pointer to the user to be removed.
     */
    void removePlayer(const shared_ptr<User>& user);

    /**
     * @brief Getter method to retrieve the game associated with the lobby.
     * @return A shared pointer to the associated BlackjackGame object.
     */
    const shared_ptr<BlackjackGame> getGame() const;

    /**
     * @brief Setter method to update the game associated with the lobby.
     * @param newGame A shared pointer to the new BlackjackGame object.
     */
    void setGame(const shared_ptr<BlackjackGame>& newGame);

    /**
     * @brief Method to send a message to all players in the lobby.
     * @param message The message to be sent.
     */
    void sendMessageToAllPlayers(const string& message);

    /**
     * @brief Method to start a game in the lobby.
     * @param opcode The operation code for the action.
     * @param clientManager A reference to the ClientManager object.
     * @param lobbyManager A reference to the LobbyManager object.
     * @return A string indicating the result of the action.
     */
    string startGame(const string& opcode, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Method to finish the game in the lobby.
     * @return A string indicating the result of finishing the game.
     */
    string finishGame();

    /**
     * @brief Method to convert lobby information to a string representation.
     * @param shouldBeLocked A flag indicating whether the lobby should be locked.
     * @param player A shared pointer to the player user for whom the string is generated.
     * @return A string representation of the lobby.
     */
    string toString(bool shouldBeLocked, shared_ptr<User> player);

    /**
     * @brief Method to convert lobby information to a string representation for LobbyManager.
     * @return A string representation of the lobby for LobbyManager.
     */
    string toLobbyManagerString();
};

#endif //SERVER_LOBBY_HPP