#pragma once

#include <string>
#include "common.hpp"

class Bid {
public:
    Bid();
    Bid(int level, Suit strain, bool isPass);

    static Bid makePass();

    int getLevel() const;
    Suit getStrain() const;
    bool isPass() const;

    // True if this bid outranks other (pass never outranks anything).
    bool isHigherThan(const Bid& other) const;

    // Level must be 1-7 and strain must be a legal denomination.
    bool isValid() const;

    std::string toString() const;   // e.g. "3 NoTrump" or "Pass"

    bool operator==(const Bid& other) const;

private:
    int level_;
    Suit strain_;
    bool isPass_;
};
