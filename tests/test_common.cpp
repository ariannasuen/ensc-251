// test_common.cpp
// Exercises every free function in common.hpp/common.cpp:
// suitToString, rankToString, nextPosition, teamOf.

#include <cassert>
#include <iostream>
#include "../common.hpp"

void test_suitToString() {
    assert(suitToString(Suit::Clubs)    == "Clubs");
    assert(suitToString(Suit::Diamonds) == "Diamonds");
    assert(suitToString(Suit::Hearts)   == "Hearts");
    assert(suitToString(Suit::Spades)   == "Spades");
    assert(suitToString(Suit::NoTrump)  == "No Trump");
    std::cout << "suitToString: OK\n";
}

void test_rankToString() {
    assert(rankToString(Rank::Two)   == "2");
    assert(rankToString(Rank::Three) == "3");
    assert(rankToString(Rank::Four)  == "4");
    assert(rankToString(Rank::Five)  == "5");
    assert(rankToString(Rank::Six)   == "6");
    assert(rankToString(Rank::Seven) == "7");
    assert(rankToString(Rank::Eight) == "8");
    assert(rankToString(Rank::Nine)  == "9");
    assert(rankToString(Rank::Ten)   == "10");
    assert(rankToString(Rank::Jack)  == "J");
    assert(rankToString(Rank::Queen) == "Q");
    assert(rankToString(Rank::King)  == "K");
    assert(rankToString(Rank::Ace)   == "A");
    std::cout << "rankToString: OK\n";
}

void test_nextPosition() {
    assert(nextPosition(Position::North) == Position::East);
    assert(nextPosition(Position::East)  == Position::South);
    assert(nextPosition(Position::South) == Position::West);
    assert(nextPosition(Position::West)  == Position::North);
    // Full cycle back to North after 4 steps.
    Position p = Position::North;
    for (int i = 0; i < 4; ++i) p = nextPosition(p);
    assert(p == Position::North);
    std::cout << "nextPosition: OK\n";
}

void test_teamOf() {
    assert(teamOf(Position::North) == Team::NorthSouth);
    assert(teamOf(Position::South) == Team::NorthSouth);
    assert(teamOf(Position::East)  == Team::EastWest);
    assert(teamOf(Position::West)  == Team::EastWest);
    std::cout << "teamOf: OK\n";
}

int main() {
    test_suitToString();
    test_rankToString();
    test_nextPosition();
    test_teamOf();
    std::cout << "\nAll common.cpp tests passed.\n";
    return 0;
}
