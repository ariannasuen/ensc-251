#include "contract.hpp"
#include "player.hpp"   // needed for declarer_->getName() in display()
#include <iostream>

Contract::Contract()
    : level_(0),
      trumpSuit_(Suit::NoTrump),
      declaringTeam_(Team::NorthSouth),
      declarer_(nullptr),
      dummy_(nullptr),
      requiredTricks_(0) {}

Contract::Contract(int level, Suit trumpSuit, Team declaringTeam,
                    Player* declarer, Player* dummy)
    : level_(level),
      trumpSuit_(trumpSuit),
      declaringTeam_(declaringTeam),
      declarer_(declarer),
      dummy_(dummy),
      requiredTricks_(0) {
    calculateRequiredTricks();
}

int Contract::getLevel() const {
    return level_;
}

Suit Contract::getTrumpSuit() const {
    return trumpSuit_;
}

Team Contract::getDeclaringTeam() const {
    return declaringTeam_;
}

Player* Contract::getDeclarer() const {
    return declarer_;
}

Player* Contract::getDummy() const {
    return dummy_;
}

int Contract::getRequiredTricks() const {
    return requiredTricks_;
}

bool Contract::isNoTrump() const {
    return trumpSuit_ == Suit::NoTrump;
}

void Contract::calculateRequiredTricks() {
    requiredTricks_ = 6 + level_;
}

void Contract::display() const {
    std::cout << level_ << " " << suitToString(trumpSuit_);
    if (declarer_ != nullptr) {
        std::cout << " by " << declarer_->getName();
    }
    std::cout << " (" << requiredTricks_ << " tricks required)" << std::endl;
}
