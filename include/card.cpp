#include "card.hpp"

Card::Card()
{
    rank = Rank::TWO;
    suit = Suit::NO_SUIT;
}

Card::Card(Rank newRank, Suit newSuit)
{
    rank = newRank;
    suit = newSuit;
}

Card::~Card()
{

}

Suit Card::getSuit() const
{
    return suit;
}

Rank Card::getRank() const
{
    return rank;
}

int Card::rankValue() const
{
    return static_cast<int>(rank);
}
