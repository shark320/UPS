#ifndef SERVER_CLIENTMANAGER_HPP
#define SERVER_CLIENTMANAGER_HPP

#include <unordered_map>
#include <string>
#include <list>
#include <mutex>
#include "User.hpp"
#include "LobbyManager.hpp"


using std::list;
using std::unordered_map;
using std::mutex;
using std::string;

class ClientInfo;

class ClientManager {
private:
    list<shared_ptr<ClientInfo>> clients;
    unordered_map<string, shared_ptr<User>> users;

public:
    ClientManager() = default;

    /**
     * @brief Adds a client to the list of clients.
     * @param socket socket of the client.
     * @param address address of the client.
     * @return A shared pointer to the client info.
    */
    shared_ptr<ClientInfo> addClient(int socket, const string& address);

    /**
     * @brief Attempts to login a user with the specified login and password. If the login is successful, the user is added to the list of clients.
     * In case of failure, the user is not added to the list of clients and a response is sent to the client with negative status and the reason for the failure.
     * @param opcode The opcode of the message. Should be 01.
     * @param client The client that sent the message.
     * @param login The login of the user.
     * @param password The password of the user.
     * @param clientManager client manager to use for working with clients.
     * @param lobbyManager lobby manager to use for working with lobbies.
     * @return A response to the client.
    */
    string attemptLogin(const string& opcode, shared_ptr<ClientInfo>& client, const string& login, const string& password, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Registers a user with the specified login, password, name and surname.
     * @param opcode The opcode of the message.
     * @param login The login of the user.
     * @param password The password of the user.
     * @param name The name of the user.
     * @param surname The surname of the user.
     * @return A response to the client.
    */
    string registerUser(const string& opcode, const string& login, const string& password, const string& name, const string& surname);

    /**
     * @brief attempts to logout the user that sent the message. If the user is not logged in, a response is sent to the client with negative status and the reason for the failure.
     * In case of success, a response is sent to the client with positive status.
     * @param opcode The opcode of the message. Should be 02.
     * @param client The client that sent the message.
     * @param clientManager client manager to use for working with clients. (in this case it is not used)
     * @param lobbyManager lobby manager to use for working with lobbies. (in this case it is not used)
     * @return message to send to the client.
    */
    string logout(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief attempts to exit the lobby the user that sent the message is in. If the user is not in a lobby, a response is sent to the client with negative status and the reason for the failure.
     * In case of success, a response is sent to the client with positive status. If the user is the admin of the lobby, the admin is changed to the next player in the lobby. If game is in progress,
     * the user is removed from the game and a message is sent to the other players in the game and game is finished.
     * @param opcode The opcode of the message. Should be 02.
     * @param client The client that sent the message.
     * @param clientManager client manager to use for working with clients.
     * @param lobbyManager lobby manager to use for working with lobbies.
     * @return message to send to the client.
    */
    string exitLobby(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief processes a message with opcode 02 (logout or exit lobby) and returns a response. And calls the appropriate function depending on the command.
     * Called functions will handle the behavior depending on the command. And send the response to the client or inform other clients if its necessary.
     * @param opcode opcode of the message. should be 02.
     * @param command command of the message. should be LOGOUT or EXITLOBBY.
     * @param client client that sent the message.
     * @param clientManager client manager to use for working with clients.
     * @param lobbyManager lobby manager to use for working with lobbies.
     * @return message to send to the client. (response)
    */
    string processLogoutOrExit(const string& opcode, const string& command, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief informs about current state of the lobby menu and the lobby the user that sent the message is in.
     * @param opcode The opcode of the message.
     * @param client The client that sent the message.
     * @param clientManager client manager to use for working with clients.
     * @param lobbyManager lobby manager to use for working with lobbies.
     * @param lobby_ptr the lobby the user was or is in.
    */
    static void informLobbyMenuAndLobby(const string& opcode, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager, shared_ptr<Lobby>& lobby_ptr);

    /**
     * @brief informs about current state of the lobby menu and the lobby the user that sent the message is in. It's an overloaded version of the function above.
     * The only difference is that it takes a user as a parameter instead of a client.
     * @param opcode The opcode of the message.
     * @param user the user because of which the notification is sent.
     * @param clientManager client manager to use for working with clients.
     * @param lobbyManager lobby manager to use for working with lobbies.
     * @param lobby_ptr the lobby the user was or is in.
    */
    void informLobbyMenuAndLobby(const string& opcode, shared_ptr<User>& user, ClientManager& clientManager, LobbyManager& lobbyManager, shared_ptr<Lobby>& lobby_ptr);

    /**
     * @brief gets the list of clients in the client manager.
     * @return The list of clients.
    */
    list<shared_ptr<ClientInfo>>& getClients();

    /**
     * @brief gets the list of users in the client manager.
     * @return The list of users.
    */
    const unordered_map<string, shared_ptr<User>>& getUsers() const;
    User getUser(const string& login) const;

    mutex client_mutex;
};

#endif //SERVER_CLIENTMANAGER_HPP
