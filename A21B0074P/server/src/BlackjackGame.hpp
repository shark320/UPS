#ifndef SERVER_BLACKJACKGAME_HPP
#define SERVER_BLACKJACKGAME_HPP

#include <vector>
#include <unordered_map>
#include "Card.hpp"
#include <random>
#include <memory>
#include <string>
#include <list>

// Forward declarations
class ClientInfo;
class ClientManager;
class LobbyManager;
class Lobby;
class User;

using std::vector;
using std::unordered_map;
using std::string;
using std::list;
using std::stringstream;
using std::weak_ptr;
using std::shared_ptr;

class BlackjackGame {
private:
    vector<Card> deck;
    unordered_map<shared_ptr<User>, vector<Card>> hands;
    weak_ptr<Lobby> lobby;
    weak_ptr<User> currentPlayer;
    weak_ptr<User> startOfRoundPlayer;
    int passCount;

    /**
     * @brief Initializes the deck with a standard set of cards.
     */
    void initializeDeck();

    /**
     * @brief Randomly shuffles the deck of cards.
     */
    void shuffleDeck();

    /**
     * @brief Deals initial cards to players.
     * @return Vector of Cards representing the initial hand.
     */
    vector<Card> dealInitialCards();

    /**
     * @brief Draws a card from the deck.
     * @return The drawn Card.
     */
    Card drawCard();

    /**
     * @brief Converts a card to a string representation.
     * @param card The Card to convert.
     * @return String representation of the card.
     */
    string cardToString(const Card& card);

    /**
     * @brief Converts a card rank to its string representation.
     * @param rank The Rank to convert.
     * @return String representation of the rank.
     */
    string cardRankToString(Rank rank);

    /**
     * @brief Converts a card suit to its string representation.
     * @param suit The Suit to convert.
     * @return String representation of the suit.
     */
    string cardSuitToString(Suit suit);

    /**
     * @brief Generates a header string of a specified length.
     * @param length The length of the header.
     * @return A string representing the generated header.
     */
    string generateHeader(int length);
    public:
    /**
     * @brief Default constructor for BlackjackGame.
     */
    BlackjackGame();

    /**
     * @brief Constructs a BlackjackGame with a reference to a Lobby.
     * @param lobby A weak pointer to the Lobby associated with the game.
     */
    explicit BlackjackGame(weak_ptr<Lobby> lobby);

    /**
     * @brief Deals cards to each player in the game.
     * @param players List of players to deal cards to.
     * @param lobby Shared pointer to the lobby.
     */
    void dealCards(const list<shared_ptr<User>>& players, shared_ptr<Lobby> lobby);

    /**
     * @brief Generates a response string for a player's turn.
     * @param playerTurnInfo Information about the player's turn.
     * @return String response for the player's turn.
     */
    string generatePlayerTurnResponse(const string& playerTurnInfo);

    /**
     * @brief Generates a response for a specific player.
     * @param recipient The player to receive the response.
     * @param players List of players in the game.
     * @param operation The operation performed.
     * @return String response for the player.
     */
    string generateResponseForPlayer(shared_ptr<User> recipient, list<shared_ptr<User>> players, const string& operation);

    /**
     * @brief Generates a response for a disconnected player.
     * @param user The user who disconnected.
     * @param operation The operation performed.
     * @return String response for the disconnected player.
     */
    string generateGameDisconnectResponse(shared_ptr<User> user, const string& operation);

    /**
     * @brief Appends player information to a response.
     * @param response The stringstream to append to.
     * @param recipient The player receiving the response.
     * @param player The player whose information is being appended.
     * @param playerIndex Index of the player.
     * @param hideCards Whether to hide the cards from the recipient.
     */
    void appendPlayerInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player, int playerIndex, bool hideCards);

    /**
     * @brief Appends card information for a player to a response.
     * @param response The stringstream to append to.
     * @param recipient The player receiving the response.
     * @param player The player whose card information is being appended.
     * @param cards Vector of Cards belonging to the player.
     * @param hideCards Whether to hide the cards from the recipient.
     */
    void appendCardInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player, const vector<Card>& cards, bool hideCards);

    /**
     * @brief Appends a response when a player has no cards.
     * @param response The stringstream to append to.
     * @param recipient The player receiving the response.
     * @param player The player without cards.
     */
    void appendNoCardInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player);

    /**
     * @brief Sends a response to a specific player.
     * @param responseStr The response string to send.
     * @param recipient The player to receive the response.
     */
    void sendResponseToPlayer(const string& responseStr, shared_ptr<User> recipient);

    /**
     * @brief Starts the game with given operation code and players.
     * @param opcode The operation code for starting the game.
      * @param players List of players participating in the game.
     * @param clientManager Reference to ClientManager for managing clients.
     * @param lobbyManager Reference to LobbyManager for managing lobbies.
     * @return String indicating the status of game start.
     */
    string startGame(const string& opcode, const list<shared_ptr<User>>& players, ClientManager& clientManager, LobbyManager& lobbyManager);

    /**
     * @brief Handles the pass action of a player.
     * @param opcode The operation code for the pass action.
     * @param player The player performing the pass action.
     * @return String response for the pass action.
     */
    string passAction(const string& opcode, shared_ptr<User> player);

    /**
     * @brief Handles the take (card draw) action of a player.
     * @param opcode The operation code for the take action.
     * @param player The player performing the take action.
     * @return String response for the take action.
     */
    string takeAction(const string& opcode, shared_ptr<User> player);

    /**
     * @brief Calculates and declares the winner of the game.
     * @return String declaring the winner.
     */
    string calculateWinner();

    /**
     * @brief Ends the current game and performs necessary cleanup.
     */
    void endGame();

    /**
     * @brief Gets the lobby associated with the game.
     * @return A weak pointer to the Lobby.
     */
    weak_ptr<Lobby> getLobby() const;

    /**
     * @brief Gets the current player in the game.
     * @return A weak pointer to the current User.
     */
    weak_ptr<User>  getCurrentPlayer() const;

    /**
     * @brief Gets the player who started the round.
     * @return A weak pointer to the User who started the round.
     */
    weak_ptr<User> getStartOfRoundPlayer() const;

    /**
     * @brief Gets the current pass count in the game.
     * @return The number of passes made in the current round.
     */
    int getPassCount() const;

    /**
     * @brief Sets the lobby associated with the game.
     * @param newLobby A weak pointer to the new Lobby to be associated with the game.
     */
    void setLobby(weak_ptr<Lobby> newLobby);

    /**
     * @brief Sets the current player in the game.
     * @param player A weak pointer to the User to be set as the current player.
     */
    void setCurrentPlayer(weak_ptr<User> player);

    /**
     * @brief Sets the player who starts the round.
     * @param player A weak pointer to the User to be set as the start of round player.
     */
    void setStartOfRoundPlayer(weak_ptr<User> player);

    /**
     * @brief Sets the pass count for the current round.
     * @param count The new pass count.
     */
    void setPassCount(int count);

    /**
     * @brief Converts the game state to a string, primarily for a given player.
     * @param player Shared pointer to the User for whom the game state is to be converted.
     * @return String representation of the game state for the specified player.
     */
    string toString(shared_ptr<User> player);

    /**
     * @brief Sends updated game information to all players after an operation.
     * @param players List of players in the game.
     * @param lobby Shared pointer to the lobby.
     * @param operation The operation that was performed.
     */
    void sendUpdatedInfo(list<shared_ptr<User>> players, shared_ptr<Lobby> lobby, const string& operation);
};

#endif //SERVER_BLACKJACKGAME_HPP