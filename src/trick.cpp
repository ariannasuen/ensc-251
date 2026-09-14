#include "trick.hpp"
#include "player.hpp"

Trick::Trick()
    : leadSuit_(Suit::NoTrump),
      trumpSuit_(Suit::NoTrump),
      leadSet_(false)
{
}

Trick::Trick(Suit trumpSuit)
    : leadSuit_(Suit::NoTrump),
      trumpSuit_(trumpSuit),
      leadSet_(false)
{
}

void Trick::addPlay(Player* player, const Card& card)
{
    if (!leadSet_)
    {
        leadSuit_ = card.getSuit();
        leadSet_ = true;
    }

    plays_.push_back(std::make_pair(player, card));
}

bool Trick::followsSuit(const Card& card) const
{
    // If no card has been played yet, there is no lead suit
    if (!leadSet_)
    {
        return true;
    }

    return card.getSuit() == leadSuit_;
}

Player* Trick::determineWinner() const
{
    if (plays_.empty())
    {
        return nullptr;
    }

    Player* winner = plays_[0].first;
    Card winningCard = plays_[0].second;

    for (const auto& play : plays_)
    {
        const Card& currentCard = play.second;

        bool currentBeatsWinner = false;

        // Case 1: Current card is trump
        if (currentCard.getSuit() == trumpSuit_)
        {
            if (winningCard.getSuit() != trumpSuit_)
            {
                currentBeatsWinner = true;
            }
            else if (currentCard.getRank() > winningCard.getRank())
            {
                currentBeatsWinner = true;
            }
        }

        // Case 2: Both cards are not trump, compare lead suit
        else if (winningCard.getSuit() != trumpSuit_)
        {
            if (currentCard.getSuit() == leadSuit_ &&
                winningCard.getSuit() != leadSuit_)
            {
                currentBeatsWinner = true;
            }
            else if (currentCard.getSuit() == leadSuit_ &&
                     winningCard.getSuit() == leadSuit_ &&
                     currentCard.getRank() > winningCard.getRank())
            {
                currentBeatsWinner = true;
            }
        }

        if (currentBeatsWinner)
        {
            winner = play.first;
            winningCard = currentCard;
        }
    }

    return winner;
}

void Trick::clear()
{
    plays_.clear();

    leadSuit_ = Suit::NoTrump;
    leadSet_ = false;
}

Suit Trick::getLeadSuit() const
{
    return leadSuit_;
}

Suit Trick::getTrumpSuit() const
{
    return trumpSuit_;
}

int Trick::cardsPlayed() const
{
    return static_cast<int>(plays_.size());
}

const std::vector<std::pair<Player*, Card>>& Trick::getPlays() const
{
    return plays_;
}
