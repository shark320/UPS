#include "Card.hpp"

Card::Card(Suit s, Rank r) : suit(s), rank(r) {}

Suit Card::getSuit() const {
    return suit;
}

Rank Card::getRank() const {
    return rank;
}

int Card::value() const {
    switch (rank) {
        case TWO: return 2;
        case THREE: return 3;
        case FOUR: return 4;
        case FIVE: return 5;
        case SIX: return 6;
        case SEVEN: return 7;
        case EIGHT: return 8;
        case NINE: return 9;
        case TEN: return 10;
        case JACK: return 10;
        case QUEEN: return 10;
        case KING: return 10;
        case ACE: return 11;
    }
    return 0; // This should not happen if all ranks are handled
}
