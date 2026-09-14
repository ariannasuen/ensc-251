#include "scoremanager.hpp"
#include "contract.hpp"
#include <iostream>

ScoreManager::ScoreManager()
    : nsScore_(0),
      ewScore_(0),
      nsTricks_(0),
      ewTricks_(0)
{
}

void ScoreManager::recordTrickWon(Team team)
{
    if (team == Team::NorthSouth)
    {
        nsTricks_++;
    }
    else
    {
        ewTricks_++;
    }
}

bool ScoreManager::contractMade(const Contract& contract) const
{
    Team declaringTeam = contract.getDeclaringTeam();

    int tricksWon;

    if (declaringTeam == Team::NorthSouth)
    {
        tricksWon = nsTricks_;
    }
    else
    {
        tricksWon = ewTricks_;
    }

    return tricksWon >= contract.getRequiredTricks();
}

namespace
{
    // Per-trick score for a made contract, standard duplicate bridge
    // trick values.
    int trickScoreFor(int level, Suit suit)
    {
        if (suit == Suit::NoTrump)
        {
            return 40 + (level - 1) * 30; // first trick 40, rest 30 each
        }
        if (suit == Suit::Hearts || suit == Suit::Spades)
        {
            return level * 30;
        }
        return level * 20; // Clubs or Diamonds
    }

    int overtrickValue(Suit suit)
    {
        return (suit == Suit::Clubs || suit == Suit::Diamonds) ? 20 : 30;
    }
}

void ScoreManager::awardPoints(const Contract& contract)
{
    Team declaringTeam = contract.getDeclaringTeam();
    int tricksWon = (declaringTeam == Team::NorthSouth) ? nsTricks_ : ewTricks_;
    int required = contract.getRequiredTricks();
    int level = contract.getLevel();
    Suit suit = contract.getTrumpSuit();

    if (contractMade(contract))
    {
        int trickScore = trickScoreFor(level, suit);
        int scoreChange = trickScore;

        // Game vs. part-score bonus.
        scoreChange += (trickScore >= 100) ? 300 : 50;

        // Slam bonuses are based on the level bid, not tricks won.
        if (level == 6)
        {
            scoreChange += 500;
        }
        else if (level == 7)
        {
            scoreChange += 1000;
        }

        int overtricks = tricksWon - required;
        if (overtricks > 0)
        {
            scoreChange += overtricks * overtrickValue(suit);
        }

        if (declaringTeam == Team::NorthSouth)
        {
            nsScore_ += scoreChange;
        }
        else
        {
            ewScore_ += scoreChange;
        }
    }
    else
    {
        int undertricks = required - tricksWon;
        int penalty = undertricks * 50;

        // Penalty points go to the defending team.
        if (declaringTeam == Team::NorthSouth)
        {
            ewScore_ += penalty;
        }
        else
        {
            nsScore_ += penalty;
        }
    }
}

int ScoreManager::getScore(Team team) const
{
    if (team == Team::NorthSouth)
    {
        return nsScore_;
    }

    return ewScore_;
}

int ScoreManager::getTricksWon(Team team) const
{
    if (team == Team::NorthSouth)
    {
        return nsTricks_;
    }

    return ewTricks_;
}

void ScoreManager::display() const
{
    std::cout << "NS Score: " << nsScore_ << "\n";
    std::cout << "EW Score: " << ewScore_ << "\n";
    std::cout << "NS Tricks: " << nsTricks_ << "\n";
    std::cout << "EW Tricks: " << ewTricks_ << "\n";
}

void ScoreManager::reset()
{
    nsTricks_ = 0;
    ewTricks_ = 0;
}
