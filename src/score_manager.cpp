#include "score_manager.hpp"
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
    if (team == Team::NS)
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
    Team declaringTeam = contract.getDeclarerTeam();

    int tricksWon;

    if (declaringTeam == Team::NS)
    {
        tricksWon = nsTricks_;
    }
    else
    {
        tricksWon = ewTricks_;
    }

    return tricksWon >= contract.getRequiredTricks();
}

void ScoreManager::awardPoints(const Contract& contract)
{
    Team declaringTeam = contract.getDeclarerTeam();

    if (contractMade(contract))
    {
        // TODO:
        // calculate bridge points from:
        // - contract level
        // - suit
        // - doubled/redoubled
        // - bonuses

        // placeholder
        if (declaringTeam == Team::NS)
        {
            nsScore_ += 1;
        }
        else
        {
            ewScore_ += 1;
        }
    }
    else
    {
        // TODO:
        // defenders receive penalty points
    }
}

int ScoreManager::getScore(Team team) const
{
    if (team == Team::NS)
    {
        return nsScore_;
    }

    return ewScore_;
}

int ScoreManager::getTricksWon(Team team) const
{
    if (team == Team::NS)
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
