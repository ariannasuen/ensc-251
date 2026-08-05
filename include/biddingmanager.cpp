#include "biddingmanager.hpp"
#include "player.hpp"
#include "contract.hpp"
#include <stdexcept>

BiddingManager::BiddingManager()
    : highestBid_(Bid::makePass()),
      highestBidder_(nullptr),
      consecutivePasses_(0) {}

void BiddingManager::reset() {
    highestBid_ = Bid::makePass();
    highestBidder_ = nullptr;
    bidHistory_.clear();
    consecutivePasses_ = 0;
}

bool BiddingManager::submitBid(Player& player, const Bid& bid) {
    if (!bid.isValid()) {
        return false;
    }
    // A non-pass bid must outbid the current highest bid.
    if (!bid.isPass() && !bid.isHigherThan(highestBid_)) {
        return false;
    }

    bidHistory_.push_back(bid);

    if (bid.isPass()) {
        consecutivePasses_++;
    } else {
        highestBid_ = bid;
        highestBidder_ = &player;
        consecutivePasses_ = 0;
    }
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

const std::vector<Bid>& BiddingManager::getBidHistory() const {
    return bidHistory_;
}

void BiddingManager::startBidding(std::vector<Player*>& players, Position dealer) {
    reset();

    if (players.empty()) {
        throw std::runtime_error("Cannot start bidding with no players");
    }

    // Locate the dealer's seat so bidding starts there and proceeds
    // in turn order.
    int startIndex = -1;
    for (std::size_t i = 0; i < players.size(); ++i) {
        if (players[i]->getPosition() == dealer) {
            startIndex = static_cast<int>(i);
            break;
        }
    }
    if (startIndex == -1) {
        throw std::runtime_error("Dealer not found among players");
    }

    int currentIndex = startIndex;
    while (!isBiddingComplete()) {
        Player* current = players[currentIndex];

        // NOTE: player.hpp as given does not declare a method for asking
        // a player to produce a bid. This assumes Player exposes something
        // like:
        //     Bid makeBid(const Bid& highestBid) const;
        // You'll need to add that (or an equivalent hook, e.g. a
        // strategy/callback the game injects) for this to compile.
        Bid bid = current->makeBid(highestBid_);

        submitBid(*current, bid);
        currentIndex = static_cast<int>((currentIndex + 1) % players.size());
    }
}

Contract BiddingManager::determineContract() const {
    if (highestBidder_ == nullptr) {
        // Everyone passed: no contract.
        return Contract();
    }

    Suit finalStrain = highestBid_.getStrain();
    Team declaringTeam = teamOf(highestBidder_->getPosition());
    Player* declarer = determineDeclarer(finalStrain, declaringTeam);

    // NOTE: determining the dummy (declarer's partner) requires looking
    // up another Player* by position/team, but BiddingManager is never
    // given the full player list (only individual Player& references via
    // submitBid). Consider either:
    //   - storing the std::vector<Player*>& passed into startBidding, or
    //   - passing the player list into determineContract explicitly,
    // so the partner can be located. Left as nullptr here.
    Player* dummy = nullptr;

    return Contract(highestBid_.getLevel(), finalStrain, declaringTeam,
                     declarer, dummy);
}

Player* BiddingManager::determineDeclarer(Suit finalStrain, Team declaringTeam) const {
    // NOTE: bidHistory_ only stores the Bid objects, not which Player
    // made each one, so we cannot walk the history to find the FIRST
    // player on declaringTeam who bid finalStrain (which is what the
    // header comment specifies). To do this properly, bidHistory_ would
    // need to become something like std::vector<std::pair<Player*, Bid>>,
    // updated in submitBid.
    //
    // As a fallback that at least satisfies "a member of the declaring
    // team who bid the winning strain", we return highestBidder_, since
    // by construction it made the highest (and therefore final) bid in
    // finalStrain and belongs to declaringTeam.
    (void)finalStrain;
    (void)declaringTeam;
    return highestBidder_;
}
