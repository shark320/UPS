#ifndef SERVER_MESSAGEPROCESSOR_HPP
#define SERVER_MESSAGEPROCESSOR_HPP

#include <unordered_map>
#include <string>
#include <memory>

using std::string;
using std::unordered_map;
using std::shared_ptr;

class ClientInfo;
class LobbyManager;
class ClientManager;

class MessageProcessor {
private:
    typedef string (*ProcessorFunc)(const string&, const string&, shared_ptr<ClientInfo>&, ClientManager&, LobbyManager&);

    static unordered_map<string, ProcessorFunc> processors;

public:
    /**
     * @brief Processes a registration message.
     * @param opcode The message opcode, expected to be "00".
     * @param message The message content for registration.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager handling the client.
     * @param lobbyManager Reference to the LobbyManager, not used in this function.
     * @return The response string after processing the message.
     */
    static string process00(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a login message.
     * @param opcode The message opcode, expected to be "01".
     * @param message The message content for login.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager handling the client.
     * @param lobbyManager Reference to the LobbyManager, not used in this function.
     * @return The response string after processing the message.
     */
    static string process01(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a logout or exit lobby message.
     * @param opcode The message opcode, expected to be "02".
     * @param message The message content for logout or exit.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager handling the client.
     * @param lobbyManager Reference to the LobbyManager to manage lobby-related actions.
     * @return The response string after processing the message.
     */
    static string process02(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a create lobby message.
     * @param opcode The message opcode, expected to be "03".
     * @param message The message content for creating a lobby.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager.
     * @param lobbyManager Reference to the LobbyManager to create a new lobby.
     * @return The response string after processing the message.
     */
    static string process03(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a join lobby message.
     * @param opcode The message opcode, expected to be "04".
     * @param message The message content for joining a lobby.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager.
     * @param lobbyManager Reference to the LobbyManager to add the player to a lobby.
     * @return The response string after processing the message.
     */
    static string process04(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a delete lobby message.
     * @param opcode The message opcode, expected to be "05".
     * @param message The message content for deleting a lobby.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager.
     * @param lobbyManager Reference to the LobbyManager to delete a lobby.
     * @return The response string after processing the message.
     */
    static string process05(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes game action messages.
     * @param opcode The message opcode, expected to be "06".
     * @param message The message content for game actions.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager.
     * @param lobbyManager Reference to the LobbyManager to manage game-related actions.
     * @return The response string after processing the message.
    */
    static string process06(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Processes a ping message.
     * @param opcode The message opcode, expected to be "07".
     * @param message The message content for ping.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager, not used in this function.
     * @param lobbyManager Reference to the LobbyManager, not used in this function.
     * @return The response string after processing the message.
    */
    static string process07(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);

     /**
     * @brief Processes the incoming message based on its opcode.
     * @param opcode The operation code of the message.
     * @param message The actual message content.
     * @param client Shared pointer to the ClientInfo of the sender.
     * @param clientManager Reference to the ClientManager handling various client-related actions.
     * @param lobbyManager Reference to the LobbyManager handling lobby-related actions.
     * @return The response string after processing the message.
    */
    static string processMessage(const string& opcode, const string& message, shared_ptr<ClientInfo>& client, ClientManager& clientManager, LobbyManager& lobbyManager);
};

#endif //SERVER_MESSAGEPROCESSOR_HPP