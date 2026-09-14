#include "biddingmanager.hpp"
#include "player.hpp"
#include "contract.hpp"

BiddingManager::BiddingManager()
    : highestBid_(Bid::makePass()),
      highestBidder_(nullptr),
      consecutivePasses_(0) {}

void BiddingManager::setPlayers(const std::vector<Player*>& players) {
    players_ = players;
}

void BiddingManager::reset() {
    highestBid_ = Bid::makePass();
    highestBidder_ = nullptr;
    bidHistory_.clear();
    consecutivePasses_ = 0;
    // players_ is intentionally left alone here; it's set separately via
    // setPlayers() and doesn't need to change between deals unless the
    // seating itself changes.
}

bool BiddingManager::submitBid(Player& player, const Bid& bid) {
    if (!bid.isValid()) {
        return false;
    }

    if (bid.isPass()) {
        bidHistory_.push_back(std::make_pair(&player, bid));
        consecutivePasses_++;
        return true;
    }

    // Normal bid: must outbid the current highest bid.
    if (!bid.isHigherThan(highestBid_)) {
        return false;
    }

    bidHistory_.push_back(std::make_pair(&player, bid));
    highestBid_ = bid;
    highestBidder_ = &player;
    consecutivePasses_ = 0;
    return true;
}

bool BiddingManager::isBiddingComplete() const {
    if (highestBidder_ == nullptr) {
        // Nobody has bid yet: bidding ends only once all four players
        // have passed with no bid on the table.
        return consecutivePasses_ >= 4;
    }
    // Once a bid has been made, three consecutive passes close it out.
    return consecutivePasses_ >= 3;
}

Bid BiddingManager::getHighestBid() const {
    return highestBid_;
}

Player* BiddingManager::getHighestBidder() const {
    return highestBidder_;
}

const std::vector<std::pair<Player*, Bid>>& BiddingManager::getBidHistory() const {
    return bidHistory_;
}

Player* BiddingManager::findPartner(Player* player) const {
    if (player == nullptr) {
        return nullptr;
    }
    Position partnerPosition = nextPosition(nextPosition(player->getPosition()));
    for (Player* candidate : players_) {
        if (candidate != nullptr && candidate->getPosition() == partnerPosition) {
            return candidate;
        }
    }
    return nullptr;
}

Contract BiddingManager::getContract() const {
    if (highestBidder_ == nullptr) {
        // Everyone passed: no contract.
        return Contract();
    }

    Suit finalStrain = highestBid_.getStrain();
    Team declaringTeam = teamOf(highestBidder_->getPosition());
    Player* declarer = determineDeclarer(finalStrain, declaringTeam);
    Player* dummy = findPartner(declarer);

    return Contract(highestBid_.getLevel(), finalStrain, declaringTeam,
                     declarer, dummy);
}

Player* BiddingManager::determineDeclarer(Suit finalStrain, Team declaringTeam) const {
    // The declarer is the FIRST player on the declaring team who bid
    // the winning strain. bidHistory_ stores (Player*, Bid) pairs in
    // order, so we can walk it and find that player directly.
    for (const auto& entry : bidHistory_) {
        Player* bidder = entry.first;
        const Bid& bid = entry.second;

        if (bidder == nullptr || !bid.isNormal()) {
            continue;
        }
        if (bid.getStrain() == finalStrain && teamOf(bidder->getPosition()) == declaringTeam) {
            return bidder;
        }
    }

    // Fallback: should not normally be reached, since highestBidder_
    // itself always satisfies the condition above.
    return highestBidder_;
}
