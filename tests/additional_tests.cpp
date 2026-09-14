// tests.cpp
//
// Standalone test suite implementing the team's test plan (20 test cases,
// grouped by Test Area). This file has its OWN main() and is compiled into
// its OWN executable, separate from the game (main.cpp)
//
// Build (from the project directory):
//   g++ -std=c++17 tests.cpp bid.cpp biddingmanager.cpp card.cpp common.cpp \
//       contract.cpp deck.cpp hand.cpp player.cpp scoremanager.cpp trick.cpp \
//       -o bridge_tests
//
// Run:
//   ./bridge_tests

#include <iostream>
#include <string>
#include <vector>

#include "common.hpp"
#include "bid.hpp"
#include "biddingmanager.hpp"
#include "card.hpp"
#include "contract.hpp"
#include "deck.hpp"
#include "hand.hpp"
#include "player.hpp"
#include "scoremanager.hpp"
#include "trick.hpp"

// ---------------------------------------------------------------------
// TSSest harness
// ---------------------------------------------------------------------
static int passedCount = 0;
static int failedCount = 0;

void check(bool condition, const std::string& testName)
{
    if (condition)
    {
        std::cout << "  [PASS] " << testName << "\n";
        passedCount++;
    }
    else
    {
        std::cout << "  [FAIL] " << testName << "\n";
        failedCount++;
    }
}

void section(const std::string& area, const std::string& testCase)
{
    std::cout << "\n[" << area << "] " << testCase << "\n";
}

// =======================================================================
// 1. Card -- Create and display cards
//    Expected: Cards store correct rank/suit and display properly
// =======================================================================
void test01_CardCreateAndDisplay()
{
    section("Card", "Create and display cards");

    Card queenOfDiamonds(Rank::Queen, Suit::Diamonds);
    check(queenOfDiamonds.getRank() == Rank::Queen, "Card stores the rank it was created with");
    check(queenOfDiamonds.getSuit() == Suit::Diamonds, "Card stores the suit it was created with");

    // "Display properly" -> the string conversions Hand::displayHand()
    // and main.cpp's cardToString() rely on.
    check(rankToString(Rank::Queen) == "Q", "rankToString displays Queen as \"Q\"");
    check(suitToString(Suit::Diamonds) == "Diamonds", "suitToString displays Diamonds as \"Diamonds\"");

    Card aceOfSpades(Rank::Ace, Suit::Spades);
    Card kingOfSpades(Rank::King, Suit::Spades);
    check(aceOfSpades.rankValue() > kingOfSpades.rankValue(), "Ace ranks above King");
}

// =======================================================================
// 2. Deck -- Create full deck
//    Expected: Deck contains 52 unique cards
// =======================================================================
void test02_DeckCreateFullDeck()
{
    section("Deck", "Create full deck");

    Deck deck;
    check(deck.size() == 52, "A freshly created deck has 52 cards");

    // Deck doesn't expose direct indexing, so deal every card out and
    // check the collected set for duplicates.
    std::vector<Card> dealtCards;
    while (!deck.isEmpty())
    {
        dealtCards.push_back(deck.dealCard());
    }
    check(dealtCards.size() == 52, "Dealing the whole deck yields 52 cards");

    bool anyDuplicate = false;
    for (size_t i = 0; i < dealtCards.size() && !anyDuplicate; i++)
    {
        for (size_t j = i + 1; j < dealtCards.size(); j++)
        {
            if (dealtCards[i].getSuit() == dealtCards[j].getSuit() &&
                dealtCards[i].getRank() == dealtCards[j].getRank())
            {
                anyDuplicate = true;
                break;
            }
        }
    }
    check(!anyDuplicate, "All 52 cards are unique rank/suit combinations");
}

// =======================================================================
// 3. Deck -- Shuffle and deal
//    Expected: Each of 4 players receives 13 cards
// =======================================================================
void test03_DeckShuffleAndDeal()
{
    section("Deck", "Shuffle and deal");

    Deck deck;
    deck.shuffle();

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);
    std::vector<Player*> players = { &north, &east, &south, &west };

    for (int round = 0; round < 13; round++)
    {
        for (Player* player : players)
        {
            player->receiveCard(deck.dealCard());
        }
    }

    check(north.getHand().size() == 13, "North receives 13 cards");
    check(east.getHand().size() == 13, "East receives 13 cards");
    check(south.getHand().size() == 13, "South receives 13 cards");
    check(west.getHand().size() == 13, "West receives 13 cards");
    check(deck.isEmpty(), "Deck is empty after dealing all cards out (4 x 13 = 52)");
}

// =======================================================================
// 4. Hand -- Add/remove cards
//    Expected: Hand size updates correctly
// =======================================================================
void test04_HandAddRemoveCards()
{
    section("Hand", "Add/remove cards");

    Hand hand;
    check(hand.isEmpty(), "A new hand starts empty");

    hand.addCard(Card(Rank::King, Suit::Hearts));
    hand.addCard(Card(Rank::Two, Suit::Clubs));
    check(hand.size() == 2, "Hand size increases as cards are added");

    hand.removeCard(0);
    check(hand.size() == 1, "Hand size decreases when a card is removed");
    check(hand.getCard(0).getSuit() == Suit::Clubs, "The correct remaining card is left after removal");
}

// =======================================================================
// 5. Hand -- Check for suit
//    Expected: Program correctly detects if player can follow suit
// =======================================================================
void test05_HandCheckForSuit()
{
    section("Hand", "Check for suit");

    Hand hand;
    hand.addCard(Card(Rank::King, Suit::Hearts));
    hand.addCard(Card(Rank::Nine, Suit::Clubs));

    check(hand.hasSuit(Suit::Clubs), "hasSuit correctly detects a suit the hand holds");
    check(!hand.hasSuit(Suit::Spades), "hasSuit correctly detects a suit the hand does NOT hold");
}

// =======================================================================
// 6. Bidding -- Valid bid entered
//    Expected: Bid is accepted if higher than current bid
// =======================================================================
void test06_BiddingValidBidEntered()
{
    section("Bidding", "Valid bid entered");

    Player north(Position::North);
    Player east(Position::East);
    BiddingManager manager;
    manager.setPlayers({ &north, &east });

    check(manager.submitBid(north, Bid(1, Suit::Clubs)), "1C is accepted as the opening bid");
    check(manager.submitBid(east, Bid(1, Suit::Hearts)), "1H is accepted (higher strain, same level)");
    check(manager.submitBid(north, Bid(2, Suit::Clubs)), "2C is accepted (higher level beats any lower-level bid)");
}

// =======================================================================
// 7. Bidding -- Invalid bid entered
//    Expected: Program rejects lower bids, invalid suits, or invalid levels
// =======================================================================
void test07_BiddingInvalidBidEntered()
{
    section("Bidding", "Invalid bid entered");

    Player north(Position::North);
    Player east(Position::East);
    BiddingManager manager;
    manager.setPlayers({ &north, &east });

    manager.submitBid(north, Bid(2, Suit::Hearts));
    check(!manager.submitBid(east, Bid(1, Suit::Spades)),
          "A lower-level bid (1S after 2H) is rejected");
    check(!manager.submitBid(east, Bid(2, Suit::Clubs)),
          "A same-level, lower-strain bid (2C after 2H) is rejected");

    Bid levelZero(0, Suit::Clubs);
    Bid levelEight(8, Suit::Clubs);
    check(!levelZero.isValid(), "Bid level 0 is invalid");
    check(!levelEight.isValid(), "Bid level 8 is invalid");

    Suit invalidStrain = static_cast<Suit>(99); // deliberately out-of-range
    Bid badSuitBid(3, invalidStrain);
    check(!badSuitBid.isValid(), "A bid with an out-of-range suit value is invalid");
    check(!manager.submitBid(east, badSuitBid), "An invalid bid is rejected by submitBid");
}

// =======================================================================
// 8. Bidding -- Three passes after a bid
//    Expected: Auction ends and final contract is set
// =======================================================================
void test08_BiddingThreePassesAfterABid()
{
    section("Bidding", "Three passes after a bid");

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);
    BiddingManager manager;
    manager.setPlayers({ &north, &east, &south, &west });

    manager.submitBid(north, Bid(1, Suit::NoTrump));
    manager.submitBid(east, Bid::makePass());
    manager.submitBid(south, Bid::makePass());
    check(!manager.isBiddingComplete(), "Auction is not yet complete after only 2 passes following the bid");
    manager.submitBid(west, Bid::makePass());

    check(manager.isBiddingComplete(), "Three consecutive passes after a bid ends the auction");

    Contract contract = manager.getContract();
    check(contract.getDeclarer() == &north, "The final contract's declarer is set correctly");
    check(contract.getLevel() == 1 && contract.getTrumpSuit() == Suit::NoTrump,
          "The final contract level/strain matches the winning bid (1NT)");
}

// =======================================================================
// 9. Contract -- Calculate required tricks
//    Expected: Required tricks = bid level + 6
// =======================================================================
void test09_ContractCalculateRequiredTricks()
{
    section("Contract", "Calculate required tricks");

    Contract oneLevel(1, Suit::Clubs, Team::NorthSouth, nullptr, nullptr);
    Contract fourLevel(4, Suit::Spades, Team::NorthSouth, nullptr, nullptr);
    Contract sevenLevel(7, Suit::NoTrump, Team::NorthSouth, nullptr, nullptr);

    check(oneLevel.getRequiredTricks() == 7, "Level 1 requires 7 tricks (1 + 6)");
    check(fourLevel.getRequiredTricks() == 10, "Level 4 requires 10 tricks (4 + 6)");
    check(sevenLevel.getRequiredTricks() == 13, "Level 7 (grand slam) requires 13 tricks (7 + 6)");
}

// =======================================================================
// 10. Contract -- Determine declarer/dummy
//     Expected: Correct player becomes declarer and partner becomes dummy
// =======================================================================
void test10_ContractDetermineDeclarerAndDummy()
{
    section("Contract", "Determine declarer/dummy");

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);
    BiddingManager manager;
    manager.setPlayers({ &north, &east, &south, &west });

    // North opens 1C. South is the FIRST player on North/South to name
    // Hearts. North later raises to 2H and becomes the highest bidder --
    // but South should still be declarer, per the "first to name the
    // winning strain" rule.
    manager.submitBid(north, Bid(1, Suit::Clubs));
    manager.submitBid(east, Bid::makePass());
    manager.submitBid(south, Bid(1, Suit::Hearts));
    manager.submitBid(west, Bid::makePass());
    manager.submitBid(north, Bid(2, Suit::Hearts));
    manager.submitBid(east, Bid::makePass());
    manager.submitBid(south, Bid::makePass());
    manager.submitBid(west, Bid::makePass());

    Contract contract = manager.getContract();
    check(contract.getDeclarer() == &south,
          "South (first to name Hearts) is declarer, not North (the higher bidder)");
    check(contract.getDummy() == &north,
          "North, the declarer's partner, becomes dummy");
}

// =======================================================================
// 11. Trick Play -- Player follows suit
//     Expected: Legal card is accepted
// =======================================================================
void test11_TrickPlayPlayerFollowsSuit()
{
    section("Trick Play", "Player follows suit");

    Hand hand;
    hand.addCard(Card(Rank::King, Suit::Hearts)); // index 0
    hand.addCard(Card(Rank::Two, Suit::Clubs));   // index 1

    check(hand.isValidPlay(0, Suit::Hearts), "Playing the led suit (Hearts) is accepted");
}

// =======================================================================
// 12. Trick Play -- Player breaks follow-suit rule
//     Expected: Illegal card is rejected
// =======================================================================
void test12_TrickPlayPlayerBreaksFollowSuitRule()
{
    section("Trick Play", "Player breaks follow-suit rule");

    Hand hand;
    hand.addCard(Card(Rank::King, Suit::Hearts)); // index 0
    hand.addCard(Card(Rank::Two, Suit::Clubs));   // index 1

    check(!hand.isValidPlay(1, Suit::Hearts),
          "Playing an off-suit card (Clubs) while holding Hearts is rejected");
}

// =======================================================================
// 13. Trick Play -- Determine trick winner
//     Expected: Highest trump wins, or highest led suit wins if no trump
// =======================================================================
void test13_TrickPlayDetermineTrickWinner()
{
    section("Trick Play", "Determine trick winner");

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);

    // Trump = Spades. South plays a low trump over a high off-suit card.
    Trick trumpTrick(Suit::Spades);
    trumpTrick.addPlay(&north, Card(Rank::Five, Suit::Clubs));
    trumpTrick.addPlay(&east,  Card(Rank::King, Suit::Clubs));
    trumpTrick.addPlay(&south, Card(Rank::Two, Suit::Spades));  // trump
    trumpTrick.addPlay(&west,  Card(Rank::Ace, Suit::Clubs));
    check(trumpTrick.determineWinner() == &south, "A low trump beats a high card in the led suit");

    // No-trump: highest card of the led suit wins; off-suit cards never win.
    Trick noTrumpTrick(Suit::NoTrump);
    noTrumpTrick.addPlay(&north, Card(Rank::Three, Suit::Hearts));
    noTrumpTrick.addPlay(&east,  Card(Rank::King, Suit::Hearts));
    noTrumpTrick.addPlay(&south, Card(Rank::Ace, Suit::Spades)); // off-suit
    noTrumpTrick.addPlay(&west,  Card(Rank::Queen, Suit::Hearts));
    check(noTrumpTrick.determineWinner() == &east, "With no trump, the highest card of the led suit wins");
}

// =======================================================================
// 14. Turn Order -- Trick winner leads next
//     Expected: Game updates next starting player correctly
// =======================================================================
void test14_TurnOrderTrickWinnerLeadsNext()
{
    section("Turn Order", "Trick winner leads next");

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);

    Trick trick(Suit::Spades);
    trick.addPlay(&north, Card(Rank::Five, Suit::Clubs));
    trick.addPlay(&east,  Card(Rank::King, Suit::Clubs));
    trick.addPlay(&south, Card(Rank::Two, Suit::Spades)); // trump wins
    trick.addPlay(&west,  Card(Rank::Ace, Suit::Clubs));

    Player* winner = trick.determineWinner();
    check(winner != nullptr, "determineWinner returns a player once the trick is complete");
    check(winner->getPosition() == Position::South,
          "The winner's position (South) is what main.cpp uses as next trick's leader");
}

// =======================================================================
// 15. Scoring -- Contract is made
//     Expected: Declarer's team receives credit/points
// =======================================================================
void test15_ScoringContractIsMade()
{
    section("Scoring", "Contract is made");

    ScoreManager sm;
    Contract contract(4, Suit::Hearts, Team::NorthSouth, nullptr, nullptr); // requires 10
    for (int i = 0; i < 10; i++) sm.recordTrickWon(Team::NorthSouth);

    check(sm.contractMade(contract), "4H making exactly is recognized as a made contract");
    sm.awardPoints(contract);
    check(sm.getScore(Team::NorthSouth) > 0, "The declaring team (NS) receives points for a made contract");
    check(sm.getScore(Team::EastWest) == 0, "The defending team (EW) receives no points on a made contract");
}

// =======================================================================
// 16. Scoring -- Contract is defeated
//     Expected: Defending team receives credit/points
// =======================================================================
void test16_ScoringContractIsDefeated()
{
    section("Scoring", "Contract is defeated");

    ScoreManager sm;
    Contract contract(4, Suit::Spades, Team::NorthSouth, nullptr, nullptr); // requires 10
    for (int i = 0; i < 8; i++) sm.recordTrickWon(Team::NorthSouth); // 2 down

    check(!sm.contractMade(contract), "4S down 2 is recognized as a defeated contract");
    sm.awardPoints(contract);
    check(sm.getScore(Team::EastWest) > 0, "The defending team (EW) receives penalty points");
    check(sm.getScore(Team::NorthSouth) == 0, "The declaring team (NS) receives no points on a defeated contract");
}

// =======================================================================
// 17. User Input -- Invalid input entered
//     Expected: Program shows an error and asks again
// =======================================================================
void test17_UserInputInvalidInputEntered()
{
    section("User Input", "Invalid input entered");

    Bid outOfRangeLevel(9, Suit::Clubs);
    check(!outOfRangeLevel.isValid(), "An out-of-range bid level fails validation (drives a re-prompt)");

    Hand hand;
    hand.addCard(Card(Rank::King, Suit::Hearts));
    check(!hand.isValidPlay(5, Suit::NoTrump),
          "An out-of-range card index fails validation (drives a re-prompt)");
}

// =======================================================================
// 18. Full Game -- Complete one full hand
//     Expected: Game runs through 13 tricks and displays final result
// =======================================================================
void test18_FullGameCompleteOneFullHand()
{
    section("Full Game", "Complete one full hand");

    // Deal a full deck to 4 players.
    Deck deck;
    deck.shuffle();
    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);
    std::vector<Player*> table = { &north, &east, &south, &west };

    for (int round = 0; round < 13; round++)
    {
        for (Player* player : table)
        {
            player->receiveCard(deck.dealCard());
        }
    }

    ScoreManager scoreManager;
    Position leadPosition = Position::North;
    int tricksPlayed = 0;

    // Play all 13 tricks: each player plays their first legal card.
    for (int trickNumber = 0; trickNumber < 13; trickNumber++)
    {
        Trick trick(Suit::NoTrump); // no-trump deal for simplicity
        Position currentPosition = leadPosition;

        for (int turn = 0; turn < 4; turn++)
        {
            Player* currentPlayer = nullptr;
            for (Player* player : table)
            {
                if (player->getPosition() == currentPosition)
                {
                    currentPlayer = player;
                    break;
                }
            }

            Hand currentHand = currentPlayer->getHand();
            int chosenIndex = -1;
            for (int i = 0; i < currentHand.size(); i++)
            {
                if (currentHand.isValidPlay(i, trick.getLeadSuit()))
                {
                    chosenIndex = i;
                    break;
                }
            }

            Card playedCard = currentPlayer->playCard(chosenIndex);
            trick.addPlay(currentPlayer, playedCard);
            currentPosition = nextPosition(currentPosition);
        }

        Player* winner = trick.determineWinner();
        scoreManager.recordTrickWon(winner->getTeam());
        leadPosition = winner->getPosition();
        tricksPlayed++;
    }

    check(tricksPlayed == 13, "The full hand plays out all 13 tricks");
    check(north.getHand().isEmpty() && east.getHand().isEmpty() &&
          south.getHand().isEmpty() && west.getHand().isEmpty(),
          "All hands are empty once the hand is complete");
    int totalTricksWon = scoreManager.getTricksWon(Team::NorthSouth) + scoreManager.getTricksWon(Team::EastWest);
    check(totalTricksWon == 13, "All 13 tricks are accounted for between the two teams (final result is tallied)");
}

// =======================================================================
// 19. Edge Case -- All players pass
//     Expected (per test plan): Program redeals or applies the chosen default rule
// =======================================================================
void test19_EdgeCaseAllPlayersPass()
{
    section("Edge Case", "All players pass");

    Player north(Position::North);
    Player east(Position::East);
    Player south(Position::South);
    Player west(Position::West);
    BiddingManager manager;
    manager.setPlayers({ &north, &east, &south, &west });

    manager.submitBid(north, Bid::makePass());
    manager.submitBid(east, Bid::makePass());
    manager.submitBid(south, Bid::makePass());
    manager.submitBid(west, Bid::makePass());

    check(manager.isBiddingComplete(), "Four passes with no bid ends the auction");
    Contract contract = manager.getContract();
    check(contract.getDeclarer() == nullptr, "An all-pass auction currently resolves to no contract (passed-out board)");
}

// =======================================================================
// 20. Edge Case -- Player has no led suit
//     Expected: Player may legally play any card
// =======================================================================
void test20_EdgeCasePlayerHasNoLedSuit()
{
    section("Edge Case", "Player has no led suit");

    Hand hand;
    hand.addCard(Card(Rank::Five, Suit::Hearts));
    hand.addCard(Card(Rank::Nine, Suit::Clubs));

    // Suit::NoTrump is the sentinel meaning "nothing has been led yet".
    check(hand.isValidPlay(0, Suit::NoTrump), "With no suit led, any card (index 0) is legal");
    check(hand.isValidPlay(1, Suit::NoTrump), "With no suit led, any card (index 1) is legal");
}

// =======================================================================
int main()
{
    std::cout << "========================================\n";
    std::cout << "          BRIDGE RULES TEST SUITE\n";
    std::cout << "========================================\n";

    test01_CardCreateAndDisplay();
    test02_DeckCreateFullDeck();
    test03_DeckShuffleAndDeal();
    test04_HandAddRemoveCards();
    test05_HandCheckForSuit();
    test06_BiddingValidBidEntered();
    test07_BiddingInvalidBidEntered();
    test08_BiddingThreePassesAfterABid();
    test09_ContractCalculateRequiredTricks();
    test10_ContractDetermineDeclarerAndDummy();
    test11_TrickPlayPlayerFollowsSuit();
    test12_TrickPlayPlayerBreaksFollowSuitRule();
    test13_TrickPlayDetermineTrickWinner();
    test14_TurnOrderTrickWinnerLeadsNext();
    test15_ScoringContractIsMade();
    test16_ScoringContractIsDefeated();
    test17_UserInputInvalidInputEntered();
    test18_FullGameCompleteOneFullHand();
    test19_EdgeCaseAllPlayersPass();
    test20_EdgeCasePlayerHasNoLedSuit();

    std::cout << "\n========================================\n";
    std::cout << passedCount << " passed, " << failedCount << " failed\n";
    std::cout << "========================================\n";

    return (failedCount == 0) ? 0 : 1;
}
