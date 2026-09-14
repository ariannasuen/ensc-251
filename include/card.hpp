#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include "common.hpp"

/*
Card class
Purpose:
Represents a single playing card in a standard 52-card deck.
Each card has a rank and a suit.

Suit and Rank are defined once, centrally, in common.hpp so that Card,
Hand, Deck, Bid, Contract, etc. all share the same enums.
*/

class Card
{
    public:
        /*
        Default constructor
        Precondition: None.
        Postcondition: Creates a default card
        */
        Card();

        /*
        Constructor with parameters
        Precondition: rank and suit are valid.
        Postcondition: Creates a card with the specified rank and suit.
        */
        Card(Rank rank, Suit suit);

        /*
        Destructor
        Precondition: None.
        Postcondition: Card object is destroyed.
        */
       ~Card();

       /*
       getSuit
       Precondition: Card object exits.
       Postcondition: Returns the suit of the card
       */
      Suit getSuit() const;

      /*getRank
      Precondition: Card object exists.
      Postcondition: Returns the rank of the card
      */
      Rank getRank() const;

      /*
      rankValue
      Precondition: Card object exists.
      Postcondition: Returns the integer value of the card's rank
      */
      int rankValue() const;

private:
    Rank rank;
    Suit suit;

};

#endif
