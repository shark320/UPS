#ifndef SERVER_CARD_HPP
#define SERVER_CARD_HPP

/**
 * @brief Enumeration for card suits.
 */
enum Suit {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

/**
 * @brief Enumeration for card ranks.
 */
enum Rank {
    TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING, ACE
};

/**
 * @brief Represents a playing card with a suit and rank.
 */
class Card {
private:
    Suit suit; // Suit of the card.
    Rank rank; // Rank of the card.

public:
    /**
     * @brief Constructor for creating a card.
     * @param s The suit of the card.
     * @param r The rank of the card.
     */
    Card(Suit s, Rank r);

    /**
     * @brief Gets the suit of the card.
     * @return The suit of this card.
     */
    Suit getSuit() const;

    /**
     * @brief Gets the rank of the card.
     * @return The rank of this card.
     */
    Rank getRank() const;

    /**
     * @brief Determines the value of the card based on its rank.
     * @return The integer value of the card.
     */
    int value() const;
};

#endif //SERVER_CARD_HPP
