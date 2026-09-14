// test_player.cpp
// Exercises every public function of Player:
// default ctor, (name, position, team) ctor, (position) ctor,
// getName, getPosition, getTeam, getHand, receiveCard, playCard,
// displayHand, hasSuit.

#include <cassert>
#include <iostream>
#include "../player.hpp"
#include "../card.hpp"

void test_defaultConstructor() {
    Player p;
    assert(p.getName() == "");
    assert(p.getPosition() == Position::North);
    assert(p.getTeam() == Team::NorthSouth);
    assert(p.getHand().isEmpty());
    std::cout << "Player default constructor: OK\n";
}

void test_fullConstructor() {
    Player p("Alice", Position::East, Team::EastWest);
    assert(p.getName() == "Alice");
    assert(p.getPosition() == Position::East);
    assert(p.getTeam() == Team::EastWest);
    std::cout << "Player(name, position, team) constructor: OK\n";
}

void test_positionOnlyConstructorAllSeats() {
    Player n(Position::North);
    Player e(Position::East);
    Player s(Position::South);
    Player w(Position::West);

    assert(n.getName() == "North");
    assert(n.getPosition() == Position::North);
    assert(n.getTeam() == Team::NorthSouth);

    assert(e.getName() == "East");
    assert(e.getPosition() == Position::East);
    assert(e.getTeam() == Team::EastWest);

    assert(s.getName() == "South");
    assert(s.getPosition() == Position::South);
    assert(s.getTeam() == Team::NorthSouth);

    assert(w.getName() == "West");
    assert(w.getPosition() == Position::West);
    assert(w.getTeam() == Team::EastWest);

    std::cout << "Player(position) constructor (all 4 seats): OK\n";
}

void test_getName() {
    Player p("Bob", Position::South, Team::NorthSouth);
    assert(p.getName() == "Bob");
    std::cout << "Player::getName: OK\n";
}

void test_getPosition() {
    Player p("Carol", Position::West, Team::EastWest);
    assert(p.getPosition() == Position::West);
    std::cout << "Player::getPosition: OK\n";
}

void test_getTeam() {
    Player p("Dave", Position::North, Team::NorthSouth);
    assert(p.getTeam() == Team::NorthSouth);
    std::cout << "Player::getTeam: OK\n";
}

void test_getHand() {
    Player p(Position::North);
    assert(p.getHand().isEmpty());
    p.receiveCard(Card(Rank::Ace, Suit::Spades));
    // getHand() returns a copy; confirm it reflects the current state.
    assert(p.getHand().size() == 1);
    std::cout << "Player::getHand: OK\n";
}

void test_receiveCard() {
    Player p(Position::East);
    p.receiveCard(Card(Rank::Seven, Suit::Diamonds));
    p.receiveCard(Card(Rank::Eight, Suit::Diamonds));
    assert(p.getHand().size() == 2);
    std::cout << "Player::receiveCard: OK\n";
}

void test_playCard() {
    Player p(Position::South);
    p.receiveCard(Card(Rank::Queen, Suit::Hearts));
    p.receiveCard(Card(Rank::King, Suit::Hearts));
    Card played = p.playCard(0);
    assert(played.getRank() == Rank::Queen);
    assert(p.getHand().size() == 1);
    assert(p.getHand().getCard(0).getRank() == Rank::King);
    std::cout << "Player::playCard: OK\n";
}

void test_displayHand() {
    Player p(Position::West);
    p.receiveCard(Card(Rank::Two, Suit::Clubs));
    // No return value; just confirm it runs without crashing.
    p.displayHand();
    std::cout << "Player::displayHand (ran without crash): OK\n";
}

void test_hasSuit() {
    Player p(Position::North);
    p.receiveCard(Card(Rank::Nine, Suit::Clubs));
    assert(p.hasSuit(Suit::Clubs));
    assert(!p.hasSuit(Suit::Hearts));
    std::cout << "Player::hasSuit: OK\n";
}

int main() {
    test_defaultConstructor();
    test_fullConstructor();
    test_positionOnlyConstructorAllSeats();
    test_getName();
    test_getPosition();
    test_getTeam();
    test_getHand();
    test_receiveCard();
    test_playCard();
    test_displayHand();
    test_hasSuit();
    std::cout << "\nAll Player tests passed.\n";
    return 0;
}
