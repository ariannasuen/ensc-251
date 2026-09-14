// test_hand.cpp
// Exercises every public function of Hand:
// constructor, addCard, removeCard (valid + out-of-range), getCard
// (valid + out-of-range), hasSuit, isValidPlay, sortHand,
// displayHand, size, isEmpty.

#include <cassert>
#include <iostream>
#include <stdexcept>
#include "../hand.hpp"
#include "../card.hpp"

void test_defaultConstructorIsEmpty() {
    Hand h;
    assert(h.isEmpty());
    assert(h.size() == 0);
    std::cout << "Hand default constructor (empty): OK\n";
}

void test_addCard() {
    Hand h;
    h.addCard(Card(Rank::Ace, Suit::Spades));
    assert(h.size() == 1);
    assert(!h.isEmpty());
    h.addCard(Card(Rank::King, Suit::Hearts));
    assert(h.size() == 2);
    std::cout << "Hand::addCard: OK\n";
}

void test_getCardValid() {
    Hand h;
    h.addCard(Card(Rank::Ten, Suit::Clubs));
    h.addCard(Card(Rank::Jack, Suit::Diamonds));
    assert(h.getCard(0).getRank() == Rank::Ten);
    assert(h.getCard(0).getSuit() == Suit::Clubs);
    assert(h.getCard(1).getRank() == Rank::Jack);
    assert(h.getCard(1).getSuit() == Suit::Diamonds);
    std::cout << "Hand::getCard (valid index): OK\n";
}

void test_getCardOutOfRangeThrows() {
    Hand h;
    h.addCard(Card(Rank::Nine, Suit::Hearts));
    bool threwNegative = false;
    bool threwTooHigh = false;
    try { h.getCard(-1); } catch (const std::out_of_range&) { threwNegative = true; }
    try { h.getCard(5); }  catch (const std::out_of_range&) { threwTooHigh = true; }
    assert(threwNegative);
    assert(threwTooHigh);
    std::cout << "Hand::getCard (out-of-range throws): OK\n";
}

void test_removeCardValid() {
    Hand h;
    h.addCard(Card(Rank::Two, Suit::Clubs));
    h.addCard(Card(Rank::Three, Suit::Clubs));
    h.addCard(Card(Rank::Four, Suit::Clubs));
    h.removeCard(1); // removes the Three
    assert(h.size() == 2);
    assert(h.getCard(0).getRank() == Rank::Two);
    assert(h.getCard(1).getRank() == Rank::Four);
    std::cout << "Hand::removeCard (valid index): OK\n";
}

void test_removeCardOutOfRangeThrows() {
    Hand h;
    h.addCard(Card(Rank::Five, Suit::Spades));
    bool threwNegative = false;
    bool threwTooHigh = false;
    try { h.removeCard(-1); } catch (const std::out_of_range&) { threwNegative = true; }
    try { h.removeCard(9); }  catch (const std::out_of_range&) { threwTooHigh = true; }
    assert(threwNegative);
    assert(threwTooHigh);
    assert(h.size() == 1); // untouched
    std::cout << "Hand::removeCard (out-of-range throws): OK\n";
}

void test_hasSuit() {
    Hand h;
    h.addCard(Card(Rank::Six, Suit::Hearts));
    h.addCard(Card(Rank::Seven, Suit::Clubs));
    assert(h.hasSuit(Suit::Hearts));
    assert(h.hasSuit(Suit::Clubs));
    assert(!h.hasSuit(Suit::Spades));
    assert(!h.hasSuit(Suit::Diamonds));
    std::cout << "Hand::hasSuit: OK\n";
}

void test_isValidPlay() {
    Hand h;
    h.addCard(Card(Rank::Eight, Suit::Hearts));  // index 0
    h.addCard(Card(Rank::Nine, Suit::Clubs));    // index 1

    // No suit led yet (NoTrump sentinel): anything is valid.
    assert(h.isValidPlay(0, Suit::NoTrump));
    assert(h.isValidPlay(1, Suit::NoTrump));

    // Lead suit is Hearts and hand has a Heart: only the Heart is valid.
    assert(h.isValidPlay(0, Suit::Hearts));
    assert(!h.isValidPlay(1, Suit::Hearts));

    // Lead suit is Diamonds and hand has none: any card is valid
    // (can't follow suit).
    assert(h.isValidPlay(0, Suit::Diamonds));
    assert(h.isValidPlay(1, Suit::Diamonds));

    // Out-of-range index is never valid.
    assert(!h.isValidPlay(-1, Suit::Hearts));
    assert(!h.isValidPlay(5, Suit::Hearts));

    std::cout << "Hand::isValidPlay: OK\n";
}

void test_sortHand() {
    Hand h;
    h.addCard(Card(Rank::King, Suit::Spades));
    h.addCard(Card(Rank::Two, Suit::Clubs));
    h.addCard(Card(Rank::Ace, Suit::Clubs));
    h.addCard(Card(Rank::Five, Suit::Diamonds));
    h.sortHand();
    // Expect suit order Clubs < Diamonds < Hearts < Spades < NoTrump,
    // and within a suit, ascending rank.
    assert(h.getCard(0).getSuit() == Suit::Clubs);
    assert(h.getCard(0).getRank() == Rank::Two);
    assert(h.getCard(1).getSuit() == Suit::Clubs);
    assert(h.getCard(1).getRank() == Rank::Ace);
    assert(h.getCard(2).getSuit() == Suit::Diamonds);
    assert(h.getCard(3).getSuit() == Suit::Spades);
    std::cout << "Hand::sortHand: OK\n";
}

void test_displayHand() {
    // No return value to assert on; just confirm it runs without
    // crashing for both an empty and a non-empty hand.
    Hand empty;
    empty.displayHand();

    Hand h;
    h.addCard(Card(Rank::Ace, Suit::Hearts));
    h.displayHand();
    std::cout << "Hand::displayHand (ran without crash): OK\n";
}

void test_sizeAndIsEmpty() {
    Hand h;
    assert(h.isEmpty());
    assert(h.size() == 0);
    h.addCard(Card(Rank::Four, Suit::Diamonds));
    assert(!h.isEmpty());
    assert(h.size() == 1);
    h.removeCard(0);
    assert(h.isEmpty());
    assert(h.size() == 0);
    std::cout << "Hand::size / Hand::isEmpty: OK\n";
}

int main() {
    test_defaultConstructorIsEmpty();
    test_addCard();
    test_getCardValid();
    test_getCardOutOfRangeThrows();
    test_removeCardValid();
    test_removeCardOutOfRangeThrows();
    test_hasSuit();
    test_isValidPlay();
    test_sortHand();
    test_displayHand();
    test_sizeAndIsEmpty();
    std::cout << "\nAll Hand tests passed.\n";
    return 0;
}
