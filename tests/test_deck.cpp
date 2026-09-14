// test_deck.cpp
// Exercises every public function of Deck:
// constructor, createDeck, shuffle, dealCard, isEmpty, size.

#include <cassert>
#include <iostream>
#include <set>
#include <string>
#include "../deck.hpp"
#include "../common.hpp"

void test_constructorBuildsFullDeck() {
    Deck d;
    assert(d.size() == 52);
    assert(!d.isEmpty());
    std::cout << "Deck constructor (52 cards): OK\n";
}

void test_createDeckHasAllUniqueCards() {
    Deck d;
    std::set<std::string> seen;
    while (!d.isEmpty()) {
        Card c = d.dealCard();
        std::string key = suitToString(c.getSuit()) + "-" +
                           rankToString(c.getRank());
        // Every card should be unique: no duplicates in a standard deck.
        assert(seen.find(key) == seen.end());
        seen.insert(key);
    }
    assert(seen.size() == 52);
    std::cout << "Deck::createDeck (52 unique cards): OK\n";
}

void test_shuffleKeepsAllCards() {
    Deck d;
    d.shuffle();
    // Shuffling must not add or remove cards.
    assert(d.size() == 52);
    std::set<std::string> seen;
    while (!d.isEmpty()) {
        Card c = d.dealCard();
        std::string key = suitToString(c.getSuit()) + "-" +
                           rankToString(c.getRank());
        seen.insert(key);
    }
    assert(seen.size() == 52);
    std::cout << "Deck::shuffle (all cards preserved): OK\n";
}

void test_dealCardRemovesOne() {
    Deck d;
    int before = d.size();
    Card dealt = d.dealCard();
    (void)dealt;
    assert(d.size() == before - 1);
    std::cout << "Deck::dealCard (removes one card): OK\n";
}

void test_dealCardOnEmptyDeckReturnsDefault() {
    Deck d;
    for (int i = 0; i < 52; ++i) {
        d.dealCard();
    }
    assert(d.isEmpty());
    // Documented behavior: dealing from an empty deck returns a
    // default-constructed Card rather than throwing/crashing.
    Card c = d.dealCard();
    assert(c.getRank() == Rank::Two);
    assert(c.getSuit() == Suit::NoTrump);
    assert(d.isEmpty());
    std::cout << "Deck::dealCard on empty deck (default card, no crash): OK\n";
}

void test_isEmpty() {
    Deck d;
    assert(!d.isEmpty());
    for (int i = 0; i < 52; ++i) {
        d.dealCard();
    }
    assert(d.isEmpty());
    std::cout << "Deck::isEmpty: OK\n";
}

void test_size() {
    Deck d;
    assert(d.size() == 52);
    d.dealCard();
    d.dealCard();
    d.dealCard();
    assert(d.size() == 49);
    std::cout << "Deck::size: OK\n";
}

void test_createDeckResetsAfterDealing() {
    Deck d;
    d.dealCard();
    d.dealCard();
    assert(d.size() == 50);
    d.createDeck();
    // Re-creating the deck should restore all 52 cards.
    assert(d.size() == 52);
    std::cout << "Deck::createDeck (resets after dealing): OK\n";
}

int main() {
    test_constructorBuildsFullDeck();
    test_createDeckHasAllUniqueCards();
    test_shuffleKeepsAllCards();
    test_dealCardRemovesOne();
    test_dealCardOnEmptyDeckReturnsDefault();
    test_isEmpty();
    test_size();
    test_createDeckResetsAfterDealing();
    std::cout << "\nAll Deck tests passed.\n";
    return 0;
}
