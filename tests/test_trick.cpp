// test_trick.cpp
// Exercises every public function of Trick:
// default ctor, (trumpSuit) ctor, addPlay, followsSuit,
// determineWinner, clear, getLeadSuit, getTrumpSuit, cardsPlayed,
// getPlays.

#include <cassert>
#include <iostream>
#include "../trick.hpp"
#include "../player.hpp"
#include "../card.hpp"

void test_defaultConstructor() {
    Trick t;
    assert(t.getLeadSuit() == Suit::NoTrump);
    assert(t.getTrumpSuit() == Suit::NoTrump);
    assert(t.cardsPlayed() == 0);
    assert(t.getPlays().empty());
    std::cout << "Trick default constructor: OK\n";
}

void test_trumpSuitConstructor() {
    Trick t(Suit::Spades);
    assert(t.getTrumpSuit() == Suit::Spades);
    assert(t.getLeadSuit() == Suit::NoTrump); // no card led yet
    std::cout << "Trick(trumpSuit) constructor: OK\n";
}

void test_addPlaySetsLeadSuit() {
    Player north(Position::North);
    Player east(Position::East);
    Trick t;
    t.addPlay(&north, Card(Rank::Five, Suit::Hearts));
    assert(t.getLeadSuit() == Suit::Hearts);
    assert(t.cardsPlayed() == 1);
    // Lead suit doesn't change on subsequent plays.
    t.addPlay(&east, Card(Rank::King, Suit::Clubs));
    assert(t.getLeadSuit() == Suit::Hearts);
    assert(t.cardsPlayed() == 2);
    std::cout << "Trick::addPlay (sets lead suit on first card only): OK\n";
}

void test_followsSuit() {
    Trick t;
    // No card played yet: anything "follows".
    assert(t.followsSuit(Card(Rank::Two, Suit::Clubs)));

    Player north(Position::North);
    t.addPlay(&north, Card(Rank::Nine, Suit::Diamonds));
    assert(t.followsSuit(Card(Rank::Two, Suit::Diamonds)));
    assert(!t.followsSuit(Card(Rank::Two, Suit::Clubs)));
    std::cout << "Trick::followsSuit: OK\n";
}

void test_determineWinnerNoPlaysReturnsNull() {
    Trick t;
    assert(t.determineWinner() == nullptr);
    std::cout << "Trick::determineWinner (no plays -> nullptr): OK\n";
}

void test_determineWinnerHighestOfLedSuitWins() {
    Player north(Position::North), east(Position::East), south(Position::South);
    Trick t; // no trump
    t.addPlay(&north, Card(Rank::Five, Suit::Diamonds));
    t.addPlay(&east, Card(Rank::King, Suit::Diamonds));
    t.addPlay(&south, Card(Rank::Two, Suit::Diamonds));
    assert(t.determineWinner() == &east);
    std::cout << "Trick::determineWinner (highest of led suit wins): OK\n";
}

void test_determineWinnerOffSuitCannotWin() {
    Player north(Position::North), east(Position::East);
    Trick t; // no trump
    t.addPlay(&north, Card(Rank::Two, Suit::Diamonds));
    // East can't follow suit, plays a high off-suit card -- still loses.
    t.addPlay(&east, Card(Rank::Ace, Suit::Clubs));
    assert(t.determineWinner() == &north);
    std::cout << "Trick::determineWinner (off-suit card can't win): OK\n";
}

void test_determineWinnerTrumpBeatsLedSuit() {
    Player north(Position::North), east(Position::East);
    Trick t(Suit::Spades); // Spades are trump
    t.addPlay(&north, Card(Rank::Ace, Suit::Diamonds));
    t.addPlay(&east, Card(Rank::Two, Suit::Spades)); // low trump still wins
    assert(t.determineWinner() == &east);
    std::cout << "Trick::determineWinner (trump beats led suit): OK\n";
}

void test_determineWinnerHigherTrumpWins() {
    Player north(Position::North), east(Position::East), south(Position::South);
    Trick t(Suit::Hearts);
    t.addPlay(&north, Card(Rank::Three, Suit::Hearts));
    t.addPlay(&east, Card(Rank::Nine, Suit::Hearts));
    t.addPlay(&south, Card(Rank::Six, Suit::Hearts));
    assert(t.determineWinner() == &east);
    std::cout << "Trick::determineWinner (higher trump wins among trumps): OK\n";
}

void test_clear() {
    Player north(Position::North);
    Trick t(Suit::Hearts);
    t.addPlay(&north, Card(Rank::Five, Suit::Hearts));
    assert(t.cardsPlayed() == 1);
    t.clear();
    assert(t.cardsPlayed() == 0);
    assert(t.getLeadSuit() == Suit::NoTrump);
    assert(t.getPlays().empty());
    // Trump suit is preserved across clear() (only plays/lead reset).
    assert(t.getTrumpSuit() == Suit::Hearts);
    std::cout << "Trick::clear: OK\n";
}

void test_getLeadSuit() {
    Trick t;
    assert(t.getLeadSuit() == Suit::NoTrump);
    Player north(Position::North);
    t.addPlay(&north, Card(Rank::Four, Suit::Spades));
    assert(t.getLeadSuit() == Suit::Spades);
    std::cout << "Trick::getLeadSuit: OK\n";
}

void test_getTrumpSuit() {
    Trick t(Suit::Clubs);
    assert(t.getTrumpSuit() == Suit::Clubs);
    std::cout << "Trick::getTrumpSuit: OK\n";
}

void test_cardsPlayed() {
    Player n(Position::North), e(Position::East);
    Trick t;
    assert(t.cardsPlayed() == 0);
    t.addPlay(&n, Card(Rank::Two, Suit::Clubs));
    assert(t.cardsPlayed() == 1);
    t.addPlay(&e, Card(Rank::Three, Suit::Clubs));
    assert(t.cardsPlayed() == 2);
    std::cout << "Trick::cardsPlayed: OK\n";
}

void test_getPlays() {
    Player n(Position::North), e(Position::East);
    Trick t;
    t.addPlay(&n, Card(Rank::Seven, Suit::Hearts));
    t.addPlay(&e, Card(Rank::Eight, Suit::Hearts));
    const auto& plays = t.getPlays();
    assert(plays.size() == 2);
    assert(plays[0].first == &n);
    assert(plays[0].second.getRank() == Rank::Seven);
    assert(plays[1].first == &e);
    assert(plays[1].second.getRank() == Rank::Eight);
    std::cout << "Trick::getPlays: OK\n";
}

int main() {
    test_defaultConstructor();
    test_trumpSuitConstructor();
    test_addPlaySetsLeadSuit();
    test_followsSuit();
    test_determineWinnerNoPlaysReturnsNull();
    test_determineWinnerHighestOfLedSuitWins();
    test_determineWinnerOffSuitCannotWin();
    test_determineWinnerTrumpBeatsLedSuit();
    test_determineWinnerHigherTrumpWins();
    test_clear();
    test_getLeadSuit();
    test_getTrumpSuit();
    test_cardsPlayed();
    test_getPlays();
    std::cout << "\nAll Trick tests passed.\n";
    return 0;
}
