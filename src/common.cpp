#include "common.hpp"

std::string suitToString(Suit suit)
{
    switch (suit)
    {
        case Suit::Clubs:    return "Clubs";
        case Suit::Diamonds: return "Diamonds";
        case Suit::Hearts:   return "Hearts";
        case Suit::Spades:   return "Spades";
        case Suit::NoTrump:  return "No Trump";
        default:             return "Unknown";
    }
}

std::string rankToString(Rank rank)
{
    switch (rank)
    {
        case Rank::Two:   return "2";
        case Rank::Three: return "3";
        case Rank::Four:  return "4";
        case Rank::Five:  return "5";
        case Rank::Six:   return "6";
        case Rank::Seven: return "7";
        case Rank::Eight: return "8";
        case Rank::Nine:  return "9";
        case Rank::Ten:   return "10";
        case Rank::Jack:  return "J";
        case Rank::Queen: return "Q";
        case Rank::King:  return "K";
        case Rank::Ace:   return "A";
        default:          return "Unknown";
    }
}

Position nextPosition(Position position)
{
    switch (position)
    {
        case Position::North: return Position::East;
        case Position::East:  return Position::South;
        case Position::South: return Position::West;
        case Position::West:  return Position::North;
        default:              return Position::North;
    }
}

Team teamOf(Position position)
{
    switch (position)
    {
        case Position::North:
        case Position::South:
            return Team::NorthSouth;

        case Position::East:
        case Position::West:
            return Team::EastWest;

        default:
            return Team::NorthSouth;
    }
}
