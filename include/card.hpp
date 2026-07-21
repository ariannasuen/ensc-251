#ifndef CARD_HPP
#define CARD_HPP

#include <string>

/*
Card class
Purpose:
Represents a single playing card in a standard 52-card deck.
Each card has a rank and a suit.
*/

enum class Suit
{
    CLUBS,
    DIAMONDS,
    HEARTS,
    SPADES,
    NO_SUIT
};

enum class Rank
{
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
};

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