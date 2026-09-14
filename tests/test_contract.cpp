// test_contract.cpp
// Exercises every public function of Contract:
// default ctor, full ctor, getLevel, getTrumpSuit, getDeclaringTeam,
// getDeclarer, getDummy, getRequiredTricks, isNoTrump, display.

#include <cassert>
#include <iostream>
#include "../contract.hpp"
#include "../player.hpp"

void test_defaultConstructor() {
    Contract c;
    assert(c.getLevel() == 0);
    assert(c.getTrumpSuit() == Suit::NoTrump);
    assert(c.getDeclaringTeam() == Team::NorthSouth);
    assert(c.getDeclarer() == nullptr);
    assert(c.getDummy() == nullptr);
    // Default constructor does not call calculateRequiredTricks(),
    // so requiredTricks_ stays at its initialized value of 0.
    assert(c.getRequiredTricks() == 0);
    std::cout << "Contract default constructor: OK\n";
}

void test_fullConstructor() {
    Player declarer(Position::North);
    Player dummy(Position::South);
    Contract c(4, Suit::Spades, Team::NorthSouth, &declarer, &dummy);
    assert(c.getLevel() == 4);
    assert(c.getTrumpSuit() == Suit::Spades);
    assert(c.getDeclaringTeam() == Team::NorthSouth);
    assert(c.getDeclarer() == &declarer);
    assert(c.getDummy() == &dummy);
    std::cout << "Contract(level, trumpSuit, team, declarer, dummy) constructor: OK\n";
}

void test_getLevel() {
    Player d1(Position::East), d2(Position::West);
    assert(Contract(1, Suit::Clubs, Team::EastWest, &d1, &d2).getLevel() == 1);
    assert(Contract(7, Suit::NoTrump, Team::EastWest, &d1, &d2).getLevel() == 7);
    std::cout << "Contract::getLevel: OK\n";
}

void test_getTrumpSuit() {
    Player d1(Position::North), d2(Position::South);
    assert(Contract(3, Suit::Hearts, Team::NorthSouth, &d1, &d2).getTrumpSuit() == Suit::Hearts);
    assert(Contract(3, Suit::NoTrump, Team::NorthSouth, &d1, &d2).getTrumpSuit() == Suit::NoTrump);
    std::cout << "Contract::getTrumpSuit: OK\n";
}

void test_getDeclaringTeam() {
    Player d1(Position::East), d2(Position::West);
    assert(Contract(2, Suit::Diamonds, Team::EastWest, &d1, &d2).getDeclaringTeam() == Team::EastWest);
    std::cout << "Contract::getDeclaringTeam: OK\n";
}

void test_getDeclarerAndDummy() {
    Player declarer(Position::West);
    Player dummy(Position::East);
    Contract c(5, Suit::Clubs, Team::EastWest, &declarer, &dummy);
    assert(c.getDeclarer() == &declarer);
    assert(c.getDummy() == &dummy);
    std::cout << "Contract::getDeclarer / getDummy: OK\n";
}

void test_getRequiredTricks() {
    Player d1(Position::North), d2(Position::South);
    assert(Contract(1, Suit::Clubs, Team::NorthSouth, &d1, &d2).getRequiredTricks() == 7);
    assert(Contract(3, Suit::Hearts, Team::NorthSouth, &d1, &d2).getRequiredTricks() == 9);
    assert(Contract(7, Suit::NoTrump, Team::NorthSouth, &d1, &d2).getRequiredTricks() == 13);
    std::cout << "Contract::getRequiredTricks: OK\n";
}

void test_isNoTrump() {
    Player d1(Position::North), d2(Position::South);
    assert(Contract(3, Suit::NoTrump, Team::NorthSouth, &d1, &d2).isNoTrump());
    assert(!Contract(3, Suit::Spades, Team::NorthSouth, &d1, &d2).isNoTrump());
    std::cout << "Contract::isNoTrump: OK\n";
}

void test_display() {
    Player declarer(Position::North);
    Player dummy(Position::South);
    Contract c(3, Suit::Hearts, Team::NorthSouth, &declarer, &dummy);
    // No return value; confirm it runs without crashing, including
    // the nullptr-declarer default-contract case.
    c.display();
    Contract empty;
    empty.display();
    std::cout << "Contract::display (ran without crash): OK\n";
}

int main() {
    test_defaultConstructor();
    test_fullConstructor();
    test_getLevel();
    test_getTrumpSuit();
    test_getDeclaringTeam();
    test_getDeclarerAndDummy();
    test_getRequiredTricks();
    test_isNoTrump();
    test_display();
    std::cout << "\nAll Contract tests passed.\n";
    return 0;
}
