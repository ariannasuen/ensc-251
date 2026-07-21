#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include <string>
#include "card.hpp"

/*
Hand class
Purpose: Represents a player's hand of cards in a card game.
*/

class Hand
{
    public:
        /*
        Default constructor
        Precondition: None.
        Postcondition: Creates an empty hand.
        */
        Hand();

        /*
        Destructor
        Precondition: None.
        Postcondition: Destroys the hand and frees memory.
        */
        ~Hand();

        /*
        addCard
        Precondition: Card must be valid.
        Postcondition: Adds a card to the hand.
        */
        void addCard(const Card& card);

        /*
        removeCard
        Precondition: Card object exists in the hand.
        Postcondition: Removes a card from the hand.
        */
        void removeCard(int index);
        
        /*
        getCards
        Precondition: Hand object exists.
        Postcondition: Returns a vector of cards in the hand.
        */
        Card getCard(int index) const;

        /*
        hasSuit
        Precondition: Hand object exists.
        Postcondition: Returns true if the hand contains a card of the specified suit, false otherwise
        */
        bool hasSuit(Suit suit) const;

        /*
        isValidPlay
        Precondition: index must be valid, leadSuit must be a valid suit.
        Postcondition: Returns true if the card at the specified index can be played, false otherwise.
        */
        bool isValidPlay(int index, Suit leadSuit) const;

        /*
        sortHand
        Precondition: Hand object exists.
        Postcondition: Sorts the cards in the hand by rank and suit.
        */
        void sortHand();

        /*displayHand
        Precondition: Hand object exists.
        Postcondition: Displays the cards in the hand.
        */
        void displayHand() const;

        /*size
        Precondition: Hand object exists.
        Postcondition: Returns the number of cards in the hand.
        */
        int size() const;

        /*
        isEmpty
        Precondition: Hand object exists.
        Postcondition: Returns true if the hand is empty, false otherwise.
        */
        bool isEmpty() const;

    private:
        std::vector<Card> cards;

};

#endif
