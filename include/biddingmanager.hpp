#pragma once

#include <vector>
#include <utility>
#include "bid.hpp"
#include "common.hpp"

class Player;
class Contract;

class BiddingManager {
public:
    BiddingManager();

    // Tells the BiddingManager which players are at the table, in any
    // order, so getContract() can later look up a declarer's partner
    // (the dummy). Call this once, after creating the players and
    // before bidding starts.
    void setPlayers(const std::vector<Player*>& players);

    // Validates and records a single bid. Returns false if the bid is
    // illegal (e.g. does not outbid the current highest bid). The
    // caller (main.cpp) is expected to prompt the human player again
    // on a false return.
    bool submitBid(Player& player, const Bid& bid);

    // True once three consecutive passes have followed a bid, or all
    // four players have passed with no bid made.
    bool isBiddingComplete() const;

    Bid getHighestBid() const;
    Player* getHighestBidder() const;

    // Full auction history, in order, as (player, bid) pairs, so the
    // declarer can be determined by who bid what and when.
    const std::vector<std::pair<Player*, Bid>>& getBidHistory() const;

    // Builds the final Contract once bidding has ended (declarer and
    // dummy resolved from the players passed to setPlayers()).
    Contract getContract() const;

    // The declarer is the first player on the declaring team who bid
    // the winning strain.
    Player* determineDeclarer(Suit finalStrain, Team declaringTeam) const;

    // Clears the auction state (highest bid, history, pass count) back
    // to a fresh start. Does not clear the players set via setPlayers().
    void reset();

private:
    Bid highestBid_;
    Player* highestBidder_;
    std::vector<std::pair<Player*, Bid>> bidHistory_;
    std::vector<Player*> players_;
    int consecutivePasses_;

    // Finds a player's partner (same team, opposite-side seat) among
    // players_. Returns nullptr if not found.
    Player* findPartner(Player* player) const;
};
