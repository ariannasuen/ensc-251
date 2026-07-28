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

    for(int suitValue = static_cast<int>(Suit::CLUBS); suitValue <= static_cast<int>(Suit::SPADES); ++suitValue)
    {
        for(int rankValue = static_cast<int>(Rank::TWO); rankValue <= static_cast<int>(Rank::ACE); ++rankValue)
        {
            Suit currentSuit = static_cast<Suit>(suitValue);
            Rank currentRank = static_cast<Rank>(rankValue);
            Card newCard(currentRank, currentSuit);
            cards.push_back(newCard);
        }
    }
}

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
        return Card();
    }

    Card dealtCard = cards.back();
    cards.pop_back();
    return dealtCard;
}

bool Deck::isEmpty() const
{
    return cards.empty();
}

int Deck::size() const
{
    return static_cast<int>(cards.size());
}

