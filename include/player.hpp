#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "hand.hpp"
#include "card.hpp"

/*
Player class
Purpose: Represents a player in the card game.
Each player has a name, position, team, and hand of cards.
*/

enum class Position
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum class Team
{
    NORTH_SOUTH,
    EAST_WEST
};

class Player
{
    public:
        /*
        Default constructor
        Precondition: None.
        Postcondition: Creates a player with default values.
        */
        Player();

        /*
        Constructor with parameters
        Precondition: name is a valid string, position and team are valid enums.
        Postcondition: Creates a player with the specified name, position, and team.
        */
        Player(const std::string& name, Position position, Team team);

        /*
        Destructor
        Precondition: None.
        Postcondition: Player object is destroyed.
        */
        ~Player();

        /*
        getName
        Precondition: Player object exists.
        Postcondition: Returns the player's name.
        */
        std::string getName() const;

        /*
        getPosition
        Precondition: Player object exists.
        Postcondition: Returns the player's position.
        */
        Position getPosition() const;

        /*
        getTeam
        Precondition: Player object exists.
        Postcondition: Returns the player's team.
        */
        Team getTeam() const;

        /*
        getHand
        Precondition: Player object exists.
        Postcondition: Returns the player's hand of cards.
        */
        Hand getHand() const;

        /*
        receiveCard
        Precondition: Card must be valid.
        Postcondition: Adds a card to the player's hand.
        */
        void receiveCard(const Card& card);

        /*
        playCard
        Precondition: Index must be valid and in the player's hand.
        Postcondition: Removes the card at the specified index from the player's hand.
        */
        Card playCard(int index);

        /*
        displayHand
        Precondition: Player object exists.
        Postcondition: Displays the player's hand of cards.
        */
        void displayHand() const;

        /*
        hasSuit
        Precondition: Player object exists.
        Postcondition: Returns true if the player's hand contains a card of the specified suit, false otherwise.
        */
        bool hasSuit(Suit suit) const;

    private:
        std::string name;
        Position position;
        Team team;
        Hand hand;
};

#endif