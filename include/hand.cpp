#include "hand.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>

static std::string suitToString(Suit suit)
{
    switch(suit)
    {
        case Suit::HEARTS:
            return "Hearts";
        case Suit::DIAMONDS:
            return "Diamonds";
        case Suit::CLUBS:
            return "Clubs";
        case Suit::SPADES:
            return "Spades";
        default:
            return "No Suit";
    }
}

static std::string rankToString(Rank rank)
{
    switch(rank)
    {
        case Rank::TWO:
            return "2";
        case Rank::THREE:
            return "3";
        case Rank::FOUR:
            return "4";
        case Rank::FIVE:
            return "5";
        case Rank::SIX:
            return "6";
        case Rank::SEVEN:
            return "7";
        case Rank::EIGHT:
            return "8";
        case Rank::NINE:
            return "9";
        case Rank::TEN:
            return "10";
        case Rank::JACK:
            return "Jack";
        case Rank::QUEEN:
            return "Queen";
        case Rank::KING:
            return "King";
        case Rank::ACE:
            return "Ace";
        default:
            return "No Rank";
    }
}

Hand::Hand()
{

}

Hand::~Hand()
{

}

void Hand::addCard(const Card& card)
{
    cards.push_back(card);
}

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
    if(index <0 || index > size())
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

    if(leadSuit == Suit::NO_SUIT)
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
        std::cout << "[" << index << "]" << rankToString(cards[index].getRank()) << " of " << suitToString(cards[index].getSuit()) << std::endl;
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





