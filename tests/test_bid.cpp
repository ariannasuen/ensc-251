// test_bid.cpp
// Exercises every public function of Bid:
// default ctor, (level, strain) ctor, makePass, getType, getLevel,
// getStrain, isPass, isNormal, isHigherThan, isValid, toString,
// operator==.

#include <cassert>
#include <iostream>
#include "../bid.hpp"

void test_defaultConstructorIsPassSentinel() {
    Bid b;
    assert(b.getType() == BidType::Pass);
    assert(b.isPass());
    assert(!b.isNormal());
    std::cout << "Bid default constructor (Pass sentinel): OK\n";
}

void test_levelStrainConstructor() {
    Bid b(4, Suit::Spades);
    assert(b.getType() == BidType::Normal);
    assert(b.getLevel() == 4);
    assert(b.getStrain() == Suit::Spades);
    assert(b.isNormal());
    assert(!b.isPass());
    std::cout << "Bid(level, strain) constructor: OK\n";
}

void test_makePass() {
    Bid p = Bid::makePass();
    assert(p.getType() == BidType::Pass);
    assert(p.isPass());
    assert(!p.isNormal());
    std::cout << "Bid::makePass: OK\n";
}

void test_getType() {
    assert(Bid(1, Suit::Clubs).getType() == BidType::Normal);
    assert(Bid::makePass().getType() == BidType::Pass);
    std::cout << "Bid::getType: OK\n";
}

void test_getLevel() {
    assert(Bid(1, Suit::Clubs).getLevel() == 1);
    assert(Bid(7, Suit::NoTrump).getLevel() == 7);
    std::cout << "Bid::getLevel: OK\n";
}

void test_getStrain() {
    assert(Bid(2, Suit::Hearts).getStrain() == Suit::Hearts);
    assert(Bid(2, Suit::NoTrump).getStrain() == Suit::NoTrump);
    std::cout << "Bid::getStrain: OK\n";
}

void test_isPass() {
    assert(Bid::makePass().isPass());
    assert(!Bid(1, Suit::Clubs).isPass());
    std::cout << "Bid::isPass: OK\n";
}

void test_isNormal() {
    assert(Bid(1, Suit::Clubs).isNormal());
    assert(!Bid::makePass().isNormal());
    std::cout << "Bid::isNormal: OK\n";
}

void test_isHigherThan() {
    // Higher level always outranks lower level.
    assert(Bid(2, Suit::Clubs).isHigherThan(Bid(1, Suit::NoTrump)));
    assert(!Bid(1, Suit::NoTrump).isHigherThan(Bid(2, Suit::Clubs)));

    // Same level: strain rank Clubs < Diamonds < Hearts < Spades < NoTrump.
    assert(Bid(3, Suit::NoTrump).isHigherThan(Bid(3, Suit::Spades)));
    assert(Bid(3, Suit::Spades).isHigherThan(Bid(3, Suit::Hearts)));
    assert(!Bid(3, Suit::Hearts).isHigherThan(Bid(3, Suit::Spades)));

    // Equal bids: neither outranks the other.
    assert(!Bid(3, Suit::Hearts).isHigherThan(Bid(3, Suit::Hearts)));

    // Any Normal bid outranks Pass / the initial sentinel.
    assert(Bid(1, Suit::Clubs).isHigherThan(Bid::makePass()));
    assert(Bid(1, Suit::Clubs).isHigherThan(Bid())); // sentinel

    // Pass never outranks anything, even another Pass.
    assert(!Bid::makePass().isHigherThan(Bid(1, Suit::Clubs)));
    assert(!Bid::makePass().isHigherThan(Bid::makePass()));

    std::cout << "Bid::isHigherThan: OK\n";
}

void test_isValid() {
    // Pass is always structurally valid.
    assert(Bid::makePass().isValid());
    assert(Bid().isValid());

    // Normal bids: level 1-7 with a legal strain are valid.
    assert(Bid(1, Suit::Clubs).isValid());
    assert(Bid(7, Suit::NoTrump).isValid());
    assert(Bid(4, Suit::Hearts).isValid());

    // Out-of-range levels are invalid.
    assert(!Bid(0, Suit::Clubs).isValid());
    assert(!Bid(8, Suit::Clubs).isValid());
    assert(!Bid(-1, Suit::Clubs).isValid());

    std::cout << "Bid::isValid: OK\n";
}

void test_toString() {
    assert(Bid::makePass().toString() == "Pass");
    assert(Bid(3, Suit::NoTrump).toString() == "3 No Trump");
    assert(Bid(1, Suit::Clubs).toString() == "1 Clubs");
    assert(Bid(7, Suit::Spades).toString() == "7 Spades");
    std::cout << "Bid::toString: OK\n";
}

void test_operatorEquals() {
    assert(Bid(3, Suit::Hearts) == Bid(3, Suit::Hearts));
    assert(!(Bid(3, Suit::Hearts) == Bid(3, Suit::Spades)));
    assert(!(Bid(3, Suit::Hearts) == Bid(4, Suit::Hearts)));
    // Any two Pass bids are equal, regardless of level/strain fields.
    assert(Bid::makePass() == Bid::makePass());
    assert(Bid::makePass() == Bid());
    assert(!(Bid::makePass() == Bid(1, Suit::Clubs)));
    std::cout << "Bid::operator==: OK\n";
}

int main() {
    test_defaultConstructorIsPassSentinel();
    test_levelStrainConstructor();
    test_makePass();
    test_getType();
    test_getLevel();
    test_getStrain();
    test_isPass();
    test_isNormal();
    test_isHigherThan();
    test_isValid();
    test_toString();
    test_operatorEquals();
    std::cout << "\nAll Bid tests passed.\n";
    return 0;
}
