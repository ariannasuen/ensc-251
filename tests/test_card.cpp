// test_card.cpp
// Exercises every public function of Card:
// default ctor, parameterized ctor, getSuit, getRank, rankValue.

#include <cassert>
#include <iostream>
#include "../card.hpp"

void test_defaultConstructor() {
    Card c;
    // Documented default: Rank::Two, Suit::NoTrump.
    assert(c.getRank() == Rank::Two);
    assert(c.getSuit() == Suit::NoTrump);
    std::cout << "Card default constructor: OK\n";
}

void test_parameterizedConstructor() {
    Card c(Rank::Queen, Suit::Diamonds);
    assert(c.getRank() == Rank::Queen);
    assert(c.getSuit() == Suit::Diamonds);
    std::cout << "Card parameterized constructor: OK\n";
}

void test_getSuit() {
    Card c1(Rank::Five, Suit::Clubs);
    Card c2(Rank::Five, Suit::Hearts);
    Card c3(Rank::Five, Suit::Spades);
    assert(c1.getSuit() == Suit::Clubs);
    assert(c2.getSuit() == Suit::Hearts);
    assert(c3.getSuit() == Suit::Spades);
    std::cout << "Card::getSuit: OK\n";
}

void test_getRank() {
    Card low(Rank::Two, Suit::Spades);
    Card mid(Rank::Jack, Suit::Spades);
    Card high(Rank::Ace, Suit::Spades);
    assert(low.getRank() == Rank::Two);
    assert(mid.getRank() == Rank::Jack);
    assert(high.getRank() == Rank::Ace);
    std::cout << "Card::getRank: OK\n";
}

void test_rankValue() {
    assert(Card(Rank::Two, Suit::Clubs).rankValue() == 2);
    assert(Card(Rank::Ten, Suit::Clubs).rankValue() == 10);
    assert(Card(Rank::Jack, Suit::Clubs).rankValue() == 11);
    assert(Card(Rank::Queen, Suit::Clubs).rankValue() == 12);
    assert(Card(Rank::King, Suit::Clubs).rankValue() == 13);
    assert(Card(Rank::Ace, Suit::Clubs).rankValue() == 14);
    // Ordering: rankValue increases monotonically with rank.
    assert(Card(Rank::Three, Suit::Clubs).rankValue() >
           Card(Rank::Two, Suit::Clubs).rankValue());
    std::cout << "Card::rankValue: OK\n";
}

void test_destructor() {
    // Just make sure construct/destruct via scope doesn't crash
    // (no manual heap allocation in Card, but this documents intent).
    {
        Card c(Rank::King, Suit::Hearts);
        (void)c;
    }
    std::cout << "Card destructor (scope exit): OK\n";
}

int main() {
    test_defaultConstructor();
    test_parameterizedConstructor();
    test_getSuit();
    test_getRank();
    test_rankValue();
    test_destructor();
    std::cout << "\nAll Card tests passed.\n";
    return 0;
}
