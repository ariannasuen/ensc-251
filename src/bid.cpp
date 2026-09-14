#include "bid.hpp"

Bid::Bid()
    : level_(0), strain_(Suit::NoTrump), type_(BidType::Pass) {}

Bid::Bid(int level, Suit strain)
    : level_(level), strain_(strain), type_(BidType::Normal) {}

Bid::Bid(int level, Suit strain, BidType type)
    : level_(level), strain_(strain), type_(type) {}

Bid Bid::makePass() {
    return Bid(0, Suit::NoTrump, BidType::Pass);
}

BidType Bid::getType() const {
    return type_;
}

int Bid::getLevel() const {
    return level_;
}

Suit Bid::getStrain() const {
    return strain_;
}

bool Bid::isPass() const {
    return type_ == BidType::Pass;
}

bool Bid::isNormal() const {
    return type_ == BidType::Normal;
}

bool Bid::isHigherThan(const Bid& other) const {
    // Only Normal bids have a level/strain to rank by. Pass (or the
    // initial "nothing bid yet" sentinel) never outranks anything
    // through this method.
    if (type_ != BidType::Normal) {
        return false;
    }
    // Any Normal bid outranks a non-bid (Pass, or the initial "nothing
    // bid yet" sentinel).
    if (other.type_ != BidType::Normal) {
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
    if (type_ != BidType::Normal) {
        // Pass is always structurally valid.
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
    switch (type_) {
        case BidType::Pass:     return "Pass";
        case BidType::Normal:
        default:
            return std::to_string(level_) + " " + suitToString(strain_);
    }
}

bool Bid::operator==(const Bid& other) const {
    if (type_ != other.type_) {
        return false;
    }
    if (type_ != BidType::Normal) {
        return true;
    }
    return level_ == other.level_ && strain_ == other.strain_;
}
