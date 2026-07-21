#pragma once

#include <vector>
#include <utility>
#include "card.hpp"
#include "common.hpp"

class Player;

class Trick {
public:
    Trick();
    explicit Trick(Suit trumpSuit);

    // Records a card played by a player. The suit of the first card
    // played sets the lead suit for the trick.
    void addPlay(Player* player, const Card& card);

    // Checks if playing this card would follow the trick's lead suit.
    bool followsSuit(const Card& card) const;

    // Determines the winning player based on lead suit, trump suit,
    // and rank of cards played.
    Player* determineWinner() const;

    void clear();

    Suit getLeadSuit() const;
    Suit getTrumpSuit() const;
    int cardsPlayed() const;

    const std::vector<std::pair<Player*, Card>>& getPlays() const;

private:
    std::vector<std::pair<Player*, Card>> plays_;
    Suit leadSuit_;
    Suit trumpSuit_;
    bool leadSet_;
};
