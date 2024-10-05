#include "BlackjackGame.hpp"
#include "Lobby.hpp"
#include "User.hpp"
#include "ClientInfo.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
#include "Constants.hpp"
#include <sys/socket.h>
#include "Utils.hpp"
#include <climits>
#include <utility>
#include "ClientManager.hpp"

using std::random_device;
using std::mt19937;
using std::shuffle;
using std::stringstream;
using std::setw;
using std::setfill;
using std::move;


BlackjackGame::BlackjackGame(weak_ptr<Lobby> lobby)
        : lobby(move(lobby)), passCount(0) {
    initializeDeck();
    shuffleDeck();
}

BlackjackGame::BlackjackGame() : passCount(0) {
    initializeDeck();
    shuffleDeck();
}

void BlackjackGame::initializeDeck() {
    deck.reserve(52);
    for (int suit = HEARTS; suit <= SPADES; ++suit) {
        for (int rank = TWO; rank <= ACE; ++rank) {
            deck.emplace_back(static_cast<Suit>(suit), static_cast<Rank>(rank));
        }
    }
}

void BlackjackGame::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

vector<Card> BlackjackGame::dealInitialCards() {
    vector<Card> initialCards;
    initialCards.push_back(drawCard());
    initialCards.push_back(drawCard());
    return initialCards;
}

Card BlackjackGame::drawCard() {
    Card drawnCard = deck.back();
    deck.pop_back();
    return drawnCard;
}

string BlackjackGame::cardToString(const Card& card) {
    stringstream ss;
    ss << cardSuitToString(card.getSuit()) << cardRankToString(card.getRank());
    return ss.str();
}

string BlackjackGame::cardRankToString(Rank rank) {
    switch (rank) {
        case TWO:   return "2";
        case THREE: return "3";
        case FOUR:  return "4";
        case FIVE:  return "5";
        case SIX:   return "6";
        case SEVEN: return "7";
        case EIGHT: return "8";
        case NINE:  return "9";
        case TEN:   return "10";
        case JACK:  return "J";
        case QUEEN: return "Q";
        case KING:  return "K";
        case ACE:   return "A";
        default:    return "?";
    }
}

string BlackjackGame::cardSuitToString(Suit suit) {
    switch (suit) {
        case HEARTS:   return "H";
        case DIAMONDS: return "D";
        case CLUBS:    return "C";
        case SPADES:   return "S";
        default:       return "?";
    }
}

string BlackjackGame::generateHeader(int length) {
    stringstream ss;
    ss << setw(4) << setfill('0') << length;
    return ss.str();
}

void BlackjackGame::dealCards(const list<std::shared_ptr<User>>& players, shared_ptr<Lobby> lobby) {
    for (auto& player : players) {
        hands[player] = dealInitialCards();
        sendUpdatedInfo(players, lobby, START_CMD);
    }
}

string BlackjackGame::generatePlayerTurnResponse(const string& playerTurnInfo) {
    stringstream response;
    response << VALID_HEADER << OPCODE_16;
    string header = generateHeader(playerTurnInfo.size());
    response << header << playerTurnInfo;
    return response.str() + '\n';
}

void BlackjackGame::appendPlayerInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player, int playerIndex, bool hideCards) {
    response << ";[" << player->getLogin() << ";" << player->getName() << ";" << player->getSurname()<< ";" << (player->getClientInfo().lock() ? "1" : "0") << ";";
    auto handIt = hands.find(player);
    if (handIt != hands.end()) {
        appendCardInfo(response, recipient, player, handIt->second, hideCards);
    } else {
        appendNoCardInfo(response, recipient, player);
    }
    response << "]";
}

void BlackjackGame::appendCardInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player, const vector<Card>& cards, bool hideCards) {
    int value = 0;
    if (player != recipient && hideCards) {
        response << "0;" << cards.size();
    } else {
        response << "1;[";
        for (size_t i = 0; i < cards.size(); ++i) {
            value += cards[i].value();
            response << cardToString(cards[i]);
            if (i < cards.size() - 1) {
                response << ";";
            }
        }
        response << "];" << value;
    }
}

void BlackjackGame::appendNoCardInfo(stringstream& response, shared_ptr<User> recipient, shared_ptr<User> player) {
    response << (player == recipient ? "1;[]" : "0;0");
}

string BlackjackGame::generateGameDisconnectResponse(shared_ptr<User> user, const string& operation) {
    stringstream response;
    response << VALID_HEADER << OPCODE_16 << operation << ";" << user->toString();

    string leaveInfoStr = response.str().substr(8);
    string header = generateHeader(leaveInfoStr.size());
    stringstream finalResponse;
    finalResponse << VALID_HEADER << OPCODE_16 << header << leaveInfoStr;

    return finalResponse.str() + '\n';
}

string BlackjackGame::generateResponseForPlayer(shared_ptr<User> recipient, list<shared_ptr<User>> players, const string& operation) {
    stringstream response;
    response << VALID_HEADER << OPCODE_16 << operation << ";" << players.size();
    int playerIndex = 0;
    for (auto& player : players) {
        if (trimCompare(operation, START_CMD) || trimCompare(operation, TAKE_CMD) || trimCompare(operation, PASS_CMD)) {
            appendPlayerInfo(response, recipient, player, playerIndex, true);
        } else {
            appendPlayerInfo(response, recipient, player, playerIndex, false);
        }
        playerIndex++;
    }
    string playerInfoStr = response.str().substr(8);

    if (trimCompare(operation, END_CMD)) {
        playerInfoStr += calculateWinner();
    }

    string header = generateHeader(playerInfoStr.size());
    stringstream finalResponse;
    finalResponse << VALID_HEADER << OPCODE_16 << header << playerInfoStr;
    return finalResponse.str() + '\n';
}

string BlackjackGame::startGame(const string& opcode, const list<shared_ptr<User>>& players, ClientManager& clientManager, LobbyManager& lobbyManager) {
    dealCards(players, lobby.lock());
    currentPlayer = lobby.lock()->getAdmin();
    startOfRoundPlayer = currentPlayer;
    passCount = 0;

    for (auto& player : players) {
        if (player != currentPlayer.lock()) {
            string playerTurnInfo = TURN + "0;" + currentPlayer.lock()->getLogin();
            string response = generatePlayerTurnResponse(playerTurnInfo);
            if (player->getClientInfo().lock()) {
                send(player->getClientInfo().lock()->getClientSocket(), response.c_str(), response.size(), MSG_NOSIGNAL);
            }
        }
    }

    string notification = generatePositiveResponse(opcode, lobbyManager.toString(true));

    for (const auto& userPair : clientManager.getUsers()) {
        auto user = userPair.second;
            if (!user->getLobby().lock()) {
                if (user->getClientInfo().lock()) {
                    send(user->getClientInfo().lock()->getClientSocket(), notification.c_str(), notification.size(), MSG_NOSIGNAL);
                }
            }
    }

    string responseForAdmin = generatePlayerTurnResponse(TURN + "1;" + currentPlayer.lock()->getLogin());
    return responseForAdmin;
}

void BlackjackGame::sendResponseToPlayer(const string& responseStr, shared_ptr<User> recipient) {
    if (recipient->getClientInfo().lock()) {
        send(recipient->getClientInfo().lock()->getClientSocket(), responseStr.c_str(), responseStr.size(), MSG_NOSIGNAL);
    }
}

string BlackjackGame::passAction(const string& opcode, shared_ptr<User> player) {
    if (currentPlayer.lock() != player) {
        return generateNegativeResponse(opcode, ITS_NOT_YOUR_TURN);
    }

    if (currentPlayer.lock() == startOfRoundPlayer.lock()) {
        passCount = 0;
    }

    if (++passCount == static_cast<int>(lobby.lock()->getPlayers().size())) {

        for (auto& recipient : lobby.lock()->getPlayers()) {
            string responseStr = generateResponseForPlayer(recipient, lobby.lock()->getPlayers(), END_CMD);
            sendResponseToPlayer(responseStr, recipient);
        }

        return lobby.lock()->finishGame();
    }

    auto lobbyShared = lobby.lock(); // Block weak_ptr one time
    if (!lobbyShared) {
        return generateNegativeResponse(opcode, ERROR);
    }

    const auto& players = lobbyShared->getPlayers();
    auto it = find(players.begin(), players.end(), player);

    if (it == players.end()) {
        return generateNegativeResponse(opcode, ERROR);
    }

    ++it;
    if (it == players.end()) {
        it = players.begin();
    }
    currentPlayer = *it;


    sendUpdatedInfo(lobby.lock()->getPlayers(), lobby.lock(), PASS_CMD);

    for (auto& playerFor : lobby.lock()->getPlayers()) {
        if (playerFor != player) {
            string playerTurnInfo = TURN + (playerFor == currentPlayer.lock() ? "1;" : "0;") + lobby.lock()->getGame()->getCurrentPlayer().lock()->getLogin();
            string response = generatePlayerTurnResponse(playerTurnInfo);
            if (playerFor->getClientInfo().lock()) {
                send(playerFor->getClientInfo().lock()->getClientSocket(), response.c_str(), response.size(), MSG_NOSIGNAL);
            }
        }
    }

    string responseForAdmin = generatePlayerTurnResponse(TURN + "0;" + lobby.lock()->getGame()->getCurrentPlayer().lock()->getLogin());

    return responseForAdmin;
}

string BlackjackGame::takeAction(const string& opcode,shared_ptr<User> player) {
    if (currentPlayer.lock() != player) {
        return generateNegativeResponse(opcode, ITS_NOT_YOUR_TURN);
    }

    if (currentPlayer.lock() == startOfRoundPlayer.lock()) {
        passCount = 0;
    }

    if (deck.empty()) {
        return generateNegativeResponse(opcode, DECK_IS_EMPTY);
    }

    hands[player].push_back(drawCard());

    int handValue = 0;
    for (const auto& card : hands[player]) {
        handValue += card.value();
    }

    auto lobbyShared = lobby.lock();
    if (!lobbyShared) {
        return generateNegativeResponse(opcode, ERROR);
    }

    const auto& players = lobbyShared->getPlayers();
    auto it = find(players.begin(), players.end(), player);

    if (it == players.end()) {
        return generateNegativeResponse(opcode, ERROR);
    }

    ++it;
    if (it == players.end()) {
        it = players.begin();
    }
    currentPlayer = *it;

    sendUpdatedInfo(lobby.lock()->getPlayers(), lobby.lock(), TAKE_CMD);

    for (auto& playerFor : lobby.lock()->getPlayers()) {
        if (playerFor != player) {
            string playerTurnInfo = TURN + (playerFor == currentPlayer.lock() ? "1;" : "0;") + lobby.lock()->getGame()->getCurrentPlayer().lock()->getLogin();
            string response = generatePlayerTurnResponse(playerTurnInfo);
            if (playerFor->getClientInfo().lock()) {
                send(playerFor->getClientInfo().lock()->getClientSocket(), response.c_str(), response.size(), MSG_NOSIGNAL);
            }
        }
    }

    string responseForAdmin = generatePlayerTurnResponse(TURN + "0;" + lobby.lock()->getGame()->getCurrentPlayer().lock()->getLogin());

    return responseForAdmin;
}

string BlackjackGame::calculateWinner() {
    int bestValidScore = 0;
    int lowestOverScore = INT_MAX;
    vector<weak_ptr<User>> winners;

    for (const auto& playerHandPair : hands) {
        int handValue = 0;
        for (const auto& card : playerHandPair.second) {
            handValue += card.value();
        }

        if (handValue <= 21 && handValue > bestValidScore) {
            bestValidScore = handValue;
        } else if (handValue > 21 && handValue < lowestOverScore) {
            lowestOverScore = handValue;
        }
    }

    for (const auto& playerHandPair : hands) {
        int handValue = 0;
        for (const auto& card : playerHandPair.second) {
            handValue += card.value();
        }

        if (handValue == bestValidScore || (bestValidScore == 0 && handValue == lowestOverScore)) {
            winners.push_back(playerHandPair.first);
        }
    }

    stringstream winnerInfo;
    winnerInfo << ";" << winners.size() << ";[";
    for (size_t i = 0; i < winners.size(); ++i) {
        winnerInfo << winners[i].lock()->getLogin();
        if (i < winners.size() - 1) {
            winnerInfo << ";";
        }
    }
    winnerInfo << "]";

    return winnerInfo.str();
}

void BlackjackGame::endGame() {
    hands.clear();

    deck.clear();

    currentPlayer.lock() = nullptr;
    startOfRoundPlayer.lock() = nullptr;
    passCount = 0;

    lobby.lock() = nullptr;
}

weak_ptr<Lobby>  BlackjackGame::getLobby() const {
    return lobby;
}

weak_ptr<User>  BlackjackGame::getCurrentPlayer() const {
    return currentPlayer;
}

weak_ptr<User> BlackjackGame::getStartOfRoundPlayer() const {
    return startOfRoundPlayer;
}

int BlackjackGame::getPassCount() const {
    return passCount;
}

void BlackjackGame::setLobby(weak_ptr<Lobby> newLobby) {
    lobby = newLobby;
}

void BlackjackGame::setCurrentPlayer(weak_ptr<User> player) {
    currentPlayer = player;
}

void BlackjackGame::setStartOfRoundPlayer(weak_ptr<User> player) {
    startOfRoundPlayer = player;
}

void BlackjackGame::setPassCount(int count) {
    passCount = count;
}

void BlackjackGame::sendUpdatedInfo(list<shared_ptr<User>> players, shared_ptr<Lobby> lobby, const string& operation) {
    for (auto& recipient : players) {
        string responseStr = generateResponseForPlayer(recipient, players, operation);
        sendResponseToPlayer(responseStr, recipient);
    }
}

string BlackjackGame::toString(shared_ptr<User> player) {
    stringstream ss;
    ss << "[";

    ss << currentPlayer.lock()->toString();
    auto lobbyPtr = lobby.lock();

    // Getting the list of all players from lobby
    auto playersList = lobbyPtr->getPlayers();

    auto findIt = std::find(playersList.begin(), playersList.end(), player);
    bool isCurrentPlayerInList = (findIt != playersList.end());

    if (isCurrentPlayerInList) {
        appendPlayerInfo(ss, player, player, std::distance(playersList.begin(), findIt), false);
    }

    int playerIndex = 0;
    for (const auto& loopPlayer : playersList) {
        if (loopPlayer != player) {
            appendPlayerInfo(ss, player, loopPlayer, playerIndex, true);
        }
        playerIndex++;
    }

    if (!playersList.empty()) {
        ss.seekp(-1, std::ios_base::end);
    }

    return ss.str();
}

