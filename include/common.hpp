#pragma once

#include <string>

// Suit of a card. NoTrump is included here as well so that Bid/Contract
// can reuse the same enum for "strain" (suit or no-trump).
enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades,
    NoTrump
};

// Rank of a card, Two through Ace.
enum class Rank {
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
    Ace
};

// Seating position at the table.
enum class Position {
    North,
    East,
    South,
    West
};

// Partnership.
enum class Team {
    NorthSouth,
    EastWest
};

// Free helper functions shared by multiple classes.
std::string suitToString(Suit suit);
std::string rankToString(Rank rank);
Position nextPosition(Position position);
Team teamOf(Position position);
