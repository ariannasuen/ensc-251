#include "bid.hpp"

Bid::Bid()
    : level_(0), strain_(Suit::NoTrump), isPass_(true) {}

Bid::Bid(int level, Suit strain, bool isPass)
    : level_(isPass ? 0 : level), strain_(strain), isPass_(isPass) {}

Bid Bid::makePass() {
    return Bid(0, Suit::NoTrump, true);
}

int Bid::getLevel() const {
    return level_;
}

Suit Bid::getStrain() const {
    return strain_;
}

bool Bid::isPass() const {
    return isPass_;
}

bool Bid::isHigherThan(const Bid& other) const {
    // A pass never outranks anything, including another pass.
    if (isPass_) {
        return false;
    }
    // Any real bid outranks a pass.
    if (other.isPass_) {
        return true;
    }
    if (level_ != other.level_) {
        return level_ > other.level_;
    }
    // Same level: compare strain rank. common.hpp declares Suit in the
    // order Clubs < Diamonds < Hearts < Spades < NoTrump, which matches
    // standard bridge bidding order, so the underlying enum values can
    // be compared directly.
    return static_cast<int>(strain_) > static_cast<int>(other.strain_);
}

bool Bid::isValid() const {
    if (isPass_) {
        return true;
    }
    if (level_ < 1 || level_ > 7) {
        return false;
    }
    switch (strain_) {
        case Suit::Clubs:
        case Suit::Diamonds:
        case Suit::Hearts:
        case Suit::Spades:
        case Suit::NoTrump:
            return true;
        default:
            return false;
    }
}

std::string Bid::toString() const {
    if (isPass_) {
        return "Pass";
    }
    return std::to_string(level_) + " " + suitToString(strain_);
}

bool Bid::operator==(const Bid& other) const {
    if (isPass_ != other.isPass_) {
        return false;
    }
    if (isPass_) {
        return true;
    }
    return level_ == other.level_ && strain_ == other.strain_;
}
