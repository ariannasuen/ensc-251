#pragma once

#include <string>
#include "common.hpp"

// What kind of call a Bid represents.
enum class BidType {
    Normal,     // a real bid, e.g. "3 NoTrump"
    Pass
};

class Bid {
public:
    // Default bid is a Pass. Also used by BiddingManager as the "no bid
    // yet" sentinel for highestBid_.
    Bid();

    // A normal contract bid, e.g. Bid(3, Suit::NoTrump).
    Bid(int level, Suit strain);

    static Bid makePass();

    BidType getType() const;
    int getLevel() const;
    Suit getStrain() const;

    bool isPass() const;
    bool isNormal() const;

    // True if this bid outranks other by level/strain. Only meaningful
    // when both bids are Normal (or when comparing a Normal bid against
    // the initial "no bid yet" Pass sentinel); Pass never reports as
    // higher than anything via this method, since it has no
    // level/strain to rank by.
    bool isHigherThan(const Bid& other) const;

    // Pass is always structurally valid. A Normal bid must have level
    // 1-7 and a legal strain.
    bool isValid() const;

    std::string toString() const;   // e.g. "3 NoTrump", "Pass"

    bool operator==(const Bid& other) const;

private:
    Bid(int level, Suit strain, BidType type);

    int level_;
    Suit strain_;
    BidType type_;
};
