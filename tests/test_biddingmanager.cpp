// test_biddingmanager.cpp
// Exercises every public function of BiddingManager:
// constructor, setPlayers, submitBid, isBiddingComplete,
// getHighestBid, getHighestBidder, getBidHistory, getContract,
// determineDeclarer, reset.

#include <cassert>
#include <iostream>
#include <vector>
#include "../biddingmanager.hpp"
#include "../player.hpp"
#include "../contract.hpp"

void test_defaultConstructor() {
    BiddingManager bm;
    assert(bm.getHighestBid().isPass());
    assert(bm.getHighestBidder() == nullptr);
    assert(bm.getBidHistory().empty());
    // No bid yet and 0 passes: not complete.
    assert(!bm.isBiddingComplete());
    std::cout << "BiddingManager default constructor: OK\n";
}

void test_setPlayers() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.setPlayers({&n, &e, &s, &w});
    // setPlayers itself has no getter, so we verify its effect
    // indirectly through getContract()/findPartner() later. Here we
    // just confirm it doesn't crash and bidding still starts clean.
    assert(bm.getBidHistory().empty());
    std::cout << "BiddingManager::setPlayers: OK\n";
}

void test_submitBidValidNormalBid() {
    Player north(Position::North);
    BiddingManager bm;
    bool ok = bm.submitBid(north, Bid(1, Suit::Clubs));
    assert(ok);
    assert(bm.getHighestBid() == Bid(1, Suit::Clubs));
    assert(bm.getHighestBidder() == &north);
    assert(bm.getBidHistory().size() == 1);
    assert(bm.getBidHistory()[0].first == &north);
    std::cout << "BiddingManager::submitBid (valid normal bid): OK\n";
}

void test_submitBidPass() {
    Player north(Position::North);
    BiddingManager bm;
    bool ok = bm.submitBid(north, Bid::makePass());
    assert(ok);
    // A pass with no prior bid does not become the highest bid.
    assert(bm.getHighestBidder() == nullptr);
    assert(bm.getBidHistory().size() == 1);
    std::cout << "BiddingManager::submitBid (pass): OK\n";
}

void test_submitBidRejectsStructurallyInvalid() {
    Player north(Position::North);
    BiddingManager bm;
    bool ok = bm.submitBid(north, Bid(0, Suit::Clubs)); // level 0 is invalid
    assert(!ok);
    assert(bm.getBidHistory().empty());
    std::cout << "BiddingManager::submitBid (rejects invalid bid): OK\n";
}

void test_submitBidRejectsNonOutbidding() {
    Player north(Position::North);
    Player east(Position::East);
    BiddingManager bm;
    assert(bm.submitBid(north, Bid(3, Suit::Hearts)));
    // East tries a lower bid: must fail.
    bool ok = bm.submitBid(east, Bid(2, Suit::NoTrump));
    assert(!ok);
    // Highest bid is unchanged.
    assert(bm.getHighestBid() == Bid(3, Suit::Hearts));
    assert(bm.getHighestBidder() == &north);
    std::cout << "BiddingManager::submitBid (rejects non-outbidding bid): OK\n";
}

void test_isBiddingCompleteNoBidAllPass() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.submitBid(n, Bid::makePass());
    bm.submitBid(e, Bid::makePass());
    bm.submitBid(s, Bid::makePass());
    assert(!bm.isBiddingComplete()); // only 3 passes, no bid yet
    bm.submitBid(w, Bid::makePass());
    assert(bm.isBiddingComplete()); // 4 passes, no bid: complete
    std::cout << "BiddingManager::isBiddingComplete (all-pass, needs 4): OK\n";
}

void test_isBiddingCompleteAfterBid() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.submitBid(n, Bid(1, Suit::Clubs));
    bm.submitBid(e, Bid::makePass());
    bm.submitBid(s, Bid::makePass());
    assert(!bm.isBiddingComplete()); // only 2 passes since the bid
    bm.submitBid(w, Bid::makePass());
    assert(bm.isBiddingComplete()); // 3 consecutive passes after a bid
    std::cout << "BiddingManager::isBiddingComplete (after bid, needs 3): OK\n";
}

void test_isBiddingCompleteResetsOnNewBid() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.submitBid(n, Bid(1, Suit::Clubs));
    bm.submitBid(e, Bid::makePass());
    bm.submitBid(s, Bid::makePass());
    // South's partner, West, rebids instead of passing -- resets the count.
    bm.submitBid(w, Bid(2, Suit::Clubs));
    assert(!bm.isBiddingComplete());
    std::cout << "BiddingManager::isBiddingComplete (new bid resets pass count): OK\n";
}

void test_getHighestBidAndBidder() {
    Player n(Position::North), e(Position::East);
    BiddingManager bm;
    bm.submitBid(n, Bid(1, Suit::Clubs));
    bm.submitBid(e, Bid(2, Suit::Diamonds));
    assert(bm.getHighestBid() == Bid(2, Suit::Diamonds));
    assert(bm.getHighestBidder() == &e);
    std::cout << "BiddingManager::getHighestBid / getHighestBidder: OK\n";
}

void test_getBidHistory() {
    Player n(Position::North), e(Position::East);
    BiddingManager bm;
    bm.submitBid(n, Bid(1, Suit::Clubs));
    bm.submitBid(e, Bid::makePass());
    const auto& history = bm.getBidHistory();
    assert(history.size() == 2);
    assert(history[0].first == &n);
    assert(history[0].second == Bid(1, Suit::Clubs));
    assert(history[1].first == &e);
    assert(history[1].second.isPass());
    std::cout << "BiddingManager::getBidHistory: OK\n";
}

void test_getContractEveryonePasses() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.setPlayers({&n, &e, &s, &w});
    bm.submitBid(n, Bid::makePass());
    bm.submitBid(e, Bid::makePass());
    bm.submitBid(s, Bid::makePass());
    bm.submitBid(w, Bid::makePass());
    Contract c = bm.getContract();
    // Default Contract: nullptr declarer/dummy, level 0.
    assert(c.getDeclarer() == nullptr);
    assert(c.getLevel() == 0);
    std::cout << "BiddingManager::getContract (everyone passes): OK\n";
}

void test_getContractAndDetermineDeclarer() {
    Player n(Position::North), e(Position::East), s(Position::South), w(Position::West);
    BiddingManager bm;
    bm.setPlayers({&n, &e, &s, &w});

    // North opens 1H, East passes, South raises to 2H (same team/strain,
    // so South is NOT declarer -- North bid Hearts first), West passes,
    // North passes, East passes.
    bm.submitBid(n, Bid(1, Suit::Hearts));
    bm.submitBid(e, Bid::makePass());
    bm.submitBid(s, Bid(2, Suit::Hearts));
    bm.submitBid(w, Bid::makePass());
    bm.submitBid(n, Bid::makePass());
    bm.submitBid(e, Bid::makePass());
    assert(bm.isBiddingComplete());

    // North is the first NS player to have bid Hearts, so North is declarer.
    Player* declarer = bm.determineDeclarer(Suit::Hearts, Team::NorthSouth);
    assert(declarer == &n);

    Contract c = bm.getContract();
    assert(c.getLevel() == 2);
    assert(c.getTrumpSuit() == Suit::Hearts);
    assert(c.getDeclaringTeam() == Team::NorthSouth);
    assert(c.getDeclarer() == &n);
    assert(c.getDummy() == &s); // partner of North
    std::cout << "BiddingManager::getContract / determineDeclarer: OK\n";
}

void test_reset() {
    Player n(Position::North), e(Position::East);
    BiddingManager bm;
    bm.setPlayers({&n, &e});
    bm.submitBid(n, Bid(1, Suit::Clubs));
    bm.submitBid(e, Bid::makePass());
    bm.reset();
    assert(bm.getHighestBid().isPass());
    assert(bm.getHighestBidder() == nullptr);
    assert(bm.getBidHistory().empty());
    assert(!bm.isBiddingComplete());
    std::cout << "BiddingManager::reset: OK\n";
}

int main() {
    test_defaultConstructor();
    test_setPlayers();
    test_submitBidValidNormalBid();
    test_submitBidPass();
    test_submitBidRejectsStructurallyInvalid();
    test_submitBidRejectsNonOutbidding();
    test_isBiddingCompleteNoBidAllPass();
    test_isBiddingCompleteAfterBid();
    test_isBiddingCompleteResetsOnNewBid();
    test_getHighestBidAndBidder();
    test_getBidHistory();
    test_getContractEveryonePasses();
    test_getContractAndDetermineDeclarer();
    test_reset();
    std::cout << "\nAll BiddingManager tests passed.\n";
    return 0;
}
