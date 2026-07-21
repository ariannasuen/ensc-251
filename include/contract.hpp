#pragma once

#include <string>
#include "common.hpp"

class Player;

class Contract {
public:
    Contract();
    Contract(int level, Suit trumpSuit, Team declaringTeam,
              Player* declarer, Player* dummy);

    int getLevel() const;
    Suit getTrumpSuit() const;
    Team getDeclaringTeam() const;
    Player* getDeclarer() const;
    Player* getDummy() const;

    // Number of tricks the declaring team must take to make the
    // contract (6 + level).
    int getRequiredTricks() const;

    bool isNoTrump() const;

    void display() const;

private:
    int level_;
    Suit trumpSuit_;
    Team declaringTeam_;
    Player* declarer_;
    Player* dummy_;
    int requiredTricks_;

    void calculateRequiredTricks();
};
