#pragma once

#include <string>

// Suit of a card. NoTrump is included here too so that Bid/Contract can
// reuse the same enum for "strain" (a suit, or no-trump). A real Card
// is never NoTrump; that value is also reused elsewhere (Hand, Trick)
// as the sentinel for "no suit has been led yet".
enum class Suit {
    Clubs,
    Diamonds,
    Hearts,
    Spades,
    NoTrump
};

// Rank of a card, Two through Ace. Numbered from 2 to match real card
// ranks, so rankValue()/getRank() comparisons behave sensibly if ever
// used for point-count or arithmetic later.
enum class Rank {
    Two = 2,
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
