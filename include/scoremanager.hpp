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

    // Applies standard duplicate bridge scoring rules for the given
    // contract result (trick score, game/part-score bonus, slam bonus,
    // overtricks, or the appropriate undertrick penalty) and updates
    // the declaring/defending team's score.
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
