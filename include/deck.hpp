#ifndef DECK_HPP
#define DECK_HPP

#include <vector>
#include "card.hpp"

/*
Deck class
Purpose:
Represents a standard deck of 52 playing cards.
Provides functionality to shuffle, draw, and deal cards.
*/

class Deck
{
    public:
        /*
        Default constructor
        Precondition: None.
        Postcondition: Creates a new deck of 52 cards.
        */
        Deck();

        /*
        Destructor
        Precondition: None.
        Postcondition: Destroys the deck and frees memory.
        */
        ~Deck();

        /*
        createDeck
        Precondition: None.
        Postcondition: Creates a standard 52-card deck.
        */
        void createDeck();

        /*
        shuffle
        Precondition: Deck object exists.
        Postcondition: Shuffles the cards in the deck.
        */
        void shuffle();

        /*
        dealCard
        Precondition: Deck is not empty.
        Postcondition: Returns and removes one card from the deck.
        */
        Card dealCard();

        /*
        isEmpty
        Precondition: Deck object exists.
        Postcondition: Returns true if the deck is empty, false otherwise.
        */
        bool isEmpty() const;

        /*size
        Precondition: Deck object exists.
        Postcondition: Returns the number of cards remaining in the deck.
        */
        int size() const;

    private:
        std::vector<Card> cards;
};

#endif