#include "deck.hpp"

#include <algorithm>
#include <random>

Deck::Deck()
{
    createDeck();
}

Deck::~Deck()
{

}

void Deck::createDeck()
{
    cards.clear();

    // loops through rank and suit to create cards with rank 1-13 in all four suits 
    for(int suitValue = static_cast<int>(Suit::Clubs); suitValue <= static_cast<int>(Suit::Spades); ++suitValue)
    {
        for(int rankValue = static_cast<int>(Rank::Two); rankValue <= static_cast<int>(Rank::Ace); ++rankValue)
        {
            Suit currentSuit = static_cast<Suit>(suitValue);
            Rank currentRank = static_cast<Rank>(rankValue);
            Card newCard(currentRank, currentSuit);
            cards.push_back(newCard);
        }
    }
}

// uses random number generator to create deck 
void Deck::shuffle()
{
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::shuffle(cards.begin(), cards.end(), randomGenerator);
}

Card Deck::dealCard()
{
    if(isEmpty())
    {
        return Card(); // returns defualt card if deck is empty 
    }

    Card dealtCard = cards.back(); // identifies
    cards.pop_back(); // removes last card
    return dealtCard; // returns last card 
}

bool Deck::isEmpty() const
{
    return cards.empty(); // checks whether deck is empty or not 
}

int Deck::size() const
{
    return static_cast<int>(cards.size()); // returns size of deck 
}
