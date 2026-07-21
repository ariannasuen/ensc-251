#pragma once

#include <vector>
#include "bid.hpp"
#include "common.hpp"

class Player;
class Contract;

class BiddingManager {
public:
    BiddingManager();

    // Runs the full auction, asking players for bids in turn order
    // starting from the dealer, until bidding is complete.
    void startBidding(std::vector<Player*>& players, Position dealer);

    // Validates and records a single bid. Returns false if the bid is
    // illegal (e.g. does not outbid the current highest bid).
    bool submitBid(Player& player, const Bid& bid);

    // True once three consecutive passes have followed a bid, or all
    // four players have passed with no bid made.
    bool isBiddingComplete() const;

    Bid getHighestBid() const;
    Player* getHighestBidder() const;
    const std::vector<Bid>& getBidHistory() const;

    // Builds the final Contract once bidding has ended.
    Contract determineContract() const;

    // The declarer is the first player on the declaring team who bid
    // the winning strain.
    Player* determineDeclarer(Suit finalStrain, Team declaringTeam) const;

    void reset();

private:
    Bid highestBid_;
    Player* highestBidder_;
    std::vector<Bid> bidHistory_;
    int consecutivePasses_;
};
