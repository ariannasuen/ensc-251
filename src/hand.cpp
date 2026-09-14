#include "hand.hpp"
#include "common.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

Hand::Hand()
{

}

Hand::~Hand()
{

}

// add single card 
void Hand::addCard(const Card& card)
{
    cards.push_back(card); 
}

// remove single card 
void Hand::removeCard(int index)
{
    if(index < 0 || index >= size())
    {
        throw std::out_of_range("Index out of range");
    }

    cards.erase(cards.begin() + index);
}


Card Hand::getCard(int index) const
{
    if(index <0 || index >= size())
    {
        throw std::out_of_range("Invalid card index");
    }

    return cards[index];
}

bool Hand::hasSuit(Suit suit) const
{
    for(int index=0; index < size(); index++)
    {
        if(cards[index].getSuit() == suit)
        {
            return true;
        }
    }
    return false;
}

bool Hand::isValidPlay(int index, Suit leadSuit) const
{
    if(index <0 || index >= size())
    {
        return false;

    }

    // Suit::NoTrump is the sentinel meaning "no suit has been led yet"
    // (a real card is never NoTrump), matching the convention used by
    // Trick for an unset lead suit.
    if(leadSuit == Suit::NoTrump)
    {
        return true;
    }

    Suit selectedSuit = cards[index].getSuit();

    if(selectedSuit == leadSuit)
    {
        return true;
    }

    if(!hasSuit(leadSuit))
    {
        return true;
    }

    return false;
}

void Hand::sortHand()
{
    std::sort(cards.begin(), cards.end(), [](const Card& firstCard, const Card& secondCard)
    {
        int firstSuit = static_cast<int>(firstCard.getSuit());
        int secondSuit = static_cast<int>(secondCard.getSuit());

        if(firstSuit != secondSuit)
        {
            return firstSuit < secondSuit;
        }

        return firstCard.rankValue() < secondCard.rankValue();
    }
    );
}

void Hand::displayHand() const
{
    if(isEmpty())
    {
        std::cout << "Hand is empty." << std::endl;
        return;
    }

    for(int index = 0; index < size(); index++)
    {
        std::cout << "[" << index << "] " << rankToString(cards[index].getRank())
                   << " of " << suitToString(cards[index].getSuit()) << std::endl;
    }
   
}

int Hand::size() const
{
    return static_cast<int>(cards.size());
}

bool Hand::isEmpty() const
{
    return cards.empty();
}
