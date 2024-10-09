#include "Socket.hpp"
#include "ClientGuard.hpp"
#include "ClientInfo.hpp"
#include "ClientManager.hpp"
#include "MessageProcessor.hpp"
#include "LobbyManager.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <cctype>
#include <mutex>
#include <unordered_map>
#include <optional>
#include <algorithm>
#include "Constants.hpp"
#include "Utils.hpp"
#include <chrono>
#include "BlackjackGame.hpp"

using std::runtime_error;
using std::exception;
using std::thread;
using std::cerr;
using std::endl;
using std::ref;
using std::to_string;
using std::lock_guard;
using std::vector;

ClientManager clientManager;
LobbyManager lobbyManager;

/**
 * @brief Closes the client connection.
 * @param client Shared pointer to the client whose connection is to be closed.
 */
void closeClientConnection(std::shared_ptr<ClientInfo> client) {
    int skt = client->getClientSocket();
    if (auto user = client->getUser().lock()) {
        user->setClientInfo(weak_ptr<ClientInfo>());
        client->setUser(nullptr);
    }
    shutdown(skt, SHUT_RDWR);
    close(skt);
}

/**
 * @brief Reads the server configuration from a file.
 * @param configFilePath Path to the configuration file.
 * @return A map with the configuration key-value pairs.
 * @throws runtime_error if the file cannot be opened or is empty.
 */
std::map<std::string, std::string> readConfig(const std::string& configFilePath) {
    std::map<std::string, std::string> config;
    std::ifstream configFile(configFilePath);

    if (!configFile) {
        throw std::runtime_error("Cannot open config file: " + configFilePath);
    }

    std::string line;
    bool isEmpty = true;

    while (std::getline(configFile, line)) {
        isEmpty = false;
        std::istringstream is_line(line);
        std::string key;
        if (std::getline(is_line, key, '=')) {
            std::string value;
            if (std::getline(is_line, value)) {
                config[key] = value;
            }
        }
    }

    if (isEmpty) {
        throw std::runtime_error("Config file is empty: " + configFilePath);
    }

    return config;
}

/**
 * @brief Handles the client connection and processes incoming messages.
 * @param client Shared pointer to the client info.
 * @param threads Reference to the map of threads handling client connections.
 */
void handle_client(shared_ptr<ClientInfo> client, unordered_map<int, thread>& threads) {
    try {
        ClientGuard guard(clientManager.getClients(), client, clientManager.client_mutex);
        int skt = client->getClientSocket();
        log("Client connected: " + client->getAddr());

        vector<char> data(DATALEN + 1, 0);
        int datalen = DATALEN;
        int res, tmp;

        do {
            tmp = recv(skt, data.data(), HEADER_SIZE, 0);
            if (tmp == 0) {
                log("Connection closed");
                closeClientConnection(client);
                break;
            }

            if (tmp == -1) {
                log("Error receiving data: " + string(strerror(errno)));
                closeClientConnection(client);
                break;
            }

            if (strncmp(data.data(), VALID_HEADER, VALID_HEADER_LENGTH) != 0) {
                log("Invalid header received");
                closeClientConnection(client);
                break;
            }

            string opcode(data.begin() + OPCODE_OFFSET, data.begin() + OPCODE_OFFSET + 2);
            if (containsOnlyControlChars(opcode)) {
                log("Invalid opcode received");
                closeClientConnection(client);
                break;
            }

            if (opcode.length() != 2) {
                log("Invalid opcode length");
                closeClientConnection(client);
                break;
            }
            log("Opcode: " + opcode);

            string data_length_str(data.begin() + DATA_LENGTH_OFFSET, data.begin() + DATA_LENGTH_OFFSET + 4);
            if (data_length_str.length() != 4) {
                log("Invalid data length string");
                closeClientConnection(client);
                break;
            }

            if (!std::all_of(data_length_str.begin(), data_length_str.end(), ::isdigit)) {
                log("Invalid data length");
                closeClientConnection(client);
                break;
            }

            datalen = stoi(data_length_str);
            log("Message will have " + to_string(datalen) + " bytes");

            data.resize(HEADER_SIZE + datalen + 1, 0);

            res = 0;
            log("Header received : " + string(data.data(), HEADER_SIZE));
            do {
                tmp = recv(skt, data.data() + res, datalen - res, 0);
                if (tmp <= 0) {
                    res = 0;
                    break;
                }
                log("Received " + to_string(tmp) + " bytes");
                res += tmp;
            } while (res < datalen);

            string message(data.begin(), data.begin() + res);
            string response = MessageProcessor::processMessage(opcode, message, client, clientManager, lobbyManager);
            if (!response.empty()) {
                send(skt, response.c_str(), response.size(), MSG_NOSIGNAL);
            }

            if (res <= 0) {
                log("Transmission ended (error code: " + to_string(errno) + "), disconnecting");
                closeClientConnection(client);

                {
                    lock_guard<mutex> lock(clientManager.client_mutex);
                    auto& clients = clientManager.getClients();
                    clients.remove_if([&client](const shared_ptr<ClientInfo>& ci) {
                        return ci->getClientSocket() == client->getClientSocket();
                    });
                }
            } else {
                data[res] = '\0';
                log("Received: " + string(data.data()));
            }
        } while (res > 0);

        lock_guard<mutex> lock(clientManager.client_mutex);
        auto it = threads.find(client->getClientSocket());
        if (it != threads.end()) {
            if (it->second.joinable()) {
                it->second.detach();
            }
            threads.erase(it);
        }
    } catch (const std::exception& e) {
        cerr << "Exception in handle_client: " << e.what() << endl;
        closeClientConnection(client);
    } catch (...) {
        cerr << "Unknown exception in handle_client" << endl;
        closeClientConnection(client);
    }
}

/**
 * @brief Notifies other players in the game about a user's disconnection.
 * @param lobby Shared pointer to the lobby where the disconnection occurred.
 * @param user Shared pointer to the user who disconnected.
 * @param command The command associated with the disconnection.
 */
void notifyOtherPlayersInGame(std::shared_ptr<Lobby> lobby, std::shared_ptr<User> user, const string& command) {
    for (auto& recipient : lobby->getPlayers()) {
        if (recipient != user) {
            string response = lobby->getGame()->generateGameDisconnectResponse(user, command);
            lobby->getGame()->sendResponseToPlayer(response, recipient);
        }
    }
}

/**
 * @brief Handles the timeout of a client connection.
 * @param user Shared pointer to the user associated with the timed-out client.
 */
void handleClientTimeout(std::shared_ptr<User> user) {
    auto lobbyPtr = user->getLobby().lock();
    if (lobbyPtr && lobbyPtr->getGame()) {
        notifyOtherPlayersInGame(lobbyPtr, user, DISCONNECT_CMD);
    }

    closeClientConnection(user->getClientInfo().lock());
    user->incrementDisconnectCounter();
}

/**
 * @brief Handles the timeout of a user not currently associated with a client.
 * @param user Shared pointer to the user who timed out.
 */
void handleUserTimeout(std::shared_ptr<User> user) {
    auto lobbyPtr = user->getLobby().lock();
    if (lobbyPtr && lobbyPtr->getGame()) {
        if (user->getDisconnectCounter() == 0) {
            log("Other users in game notified");
            notifyOtherPlayersInGame(lobbyPtr, user, DISCONNECT_CMD);
        }
    }

    if (user->getDisconnectCounter() == 0) {
        log("Disconnected counter incremented");
        user->incrementDisconnectCounter();
    }
}

/**
 * @brief Handles the disconnection of a user from a game.
 * @param user Shared pointer to the user who disconnected.
 * @param lobby Shared pointer to the lobby from which the user disconnected.
 */
void handleGameDisconnection(std::shared_ptr<User> user, std::shared_ptr<Lobby> lobby) {
    lobby->removePlayer(user);
    user->getLobby().lock().reset();

    if (lobby->getGame()) {
        notifyOtherPlayersInGame(lobby, user, DISCONNECT_END_CMD);
        lobby->finishGame();

        if (lobby->getPlayers().empty()) {
            lobby->setAdmin(nullptr);
        } else {
            lobby->setAdmin(lobby->getPlayers().front());
        }

        clientManager.informLobbyMenuAndLobby("02", user, clientManager, lobbyManager, lobby);
    }
}

/**
 * @brief Handles the disconnection of a user.
 * @param user Shared pointer to the user who disconnected.
 */
void handleUserDisconnection(std::shared_ptr<User> user) {
    if (user->getDisconnectCounter() > 0) {
        auto lobbyPtr = user->getLobby().lock();
        if (lobbyPtr && lobbyPtr->getGame()) {
            if (user->getDisconnectCounter() >= 4) {
                handleGameDisconnection(user, lobbyPtr);
                user->setDisconnectCounter(0);
            }
            log("Disconnected counter less than 4, so incrementing");
            user->incrementDisconnectCounter();
        } else if (lobbyPtr && user->getDisconnectCounter() == 1) {
            clientManager.informLobbyMenuAndLobby("02", user, clientManager, lobbyManager, lobbyPtr);
            user->incrementDisconnectCounter();
        }
    }
}

/**
 * @brief Periodically checks for client timeouts.
 */
void checkClientsTimeout() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::lock_guard<std::mutex> lock(clientManager.client_mutex);
        auto& users = clientManager.getUsers();

        for (auto& userPair : users) {
            auto user = userPair.second;
            auto client = user->getClientInfo().lock();

            if (client && user->isPingTimeout()) {
                log("Client " + client->getAddr() + " timed out, disconnecting");
                handleClientTimeout(user);
            } else if (!client && user->isPingTimeout()){
                handleUserTimeout(user);
            }

            handleUserDisconnection(user);
        }
    }
}


/**
 * @brief The main function that initializes the server and handles incoming connections.
 * @return Exit code of the application.
 */
int main() {
    try {
        auto config = readConfig("config.txt");
        int serverPort;

        if (config.find("server_port") != config.end()) {
            serverPort = std::stoi(config["server_port"]);
        } else {
            throw std::runtime_error("Server port not defined in config");
        }

        Socket skt(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        int param = 1;
        if (setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &param, sizeof(int)) != 0) {
            throw runtime_error("Unable to set socket options");
        }

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(serverPort);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (::bind(skt, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) != 0) {
            throw runtime_error("Unable to bind to the specified address");
        }

        if (listen(skt, 10) != 0) {
            throw runtime_error("Unable to create connection queue");
        }

        sockaddr_in incoming;
        socklen_t inlen = sizeof(incoming);

        log("\nBlack Jack Server 1.0\nAkhramchuk Andrei\n> Listening on 0.0.0.0:9999");

        unordered_map<int, thread> threads;

        std::thread timeoutChecker(checkClientsTimeout);
        timeoutChecker.detach();

        while (true) {
            int inskt = accept(skt, reinterpret_cast<struct sockaddr*>(&incoming), &inlen);
            if (inskt >= 0) {
                char client_addr[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET, &incoming.sin_addr.s_addr, client_addr, INET6_ADDRSTRLEN);

                auto client = clientManager.addClient(inskt, client_addr);
                threads[inskt] = thread(handle_client, client, ref(threads));
            }
        }


    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

}
