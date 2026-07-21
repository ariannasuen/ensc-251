#pragma once

#include "common.hpp"

class Contract;

class ScoreManager {
public:
    ScoreManager();

    void recordTrickWon(Team team);

    // Compares tricks won by the declaring team against the contract's
    // required tricks.
    bool contractMade(const Contract& contract) const;

    // Applies standard bridge scoring rules for the given contract
    // result and updates the appropriate team's score.
    void awardPoints(const Contract& contract);

    int getScore(Team team) const;
    int getTricksWon(Team team) const;

    void display() const;
    void reset();   // clears trick counts between hands (scores persist)

private:
    int nsScore_;
    int ewScore_;
    int nsTricks_;
    int ewTricks_;
};
