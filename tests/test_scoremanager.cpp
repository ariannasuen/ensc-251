// test_scoremanager.cpp
// Exercises every public function of ScoreManager:
// constructor, recordTrickWon, contractMade, awardPoints
// (part-score, game, slam, grand slam, overtricks, undertrick
// penalty branches), getScore, getTricksWon, display, reset.

#include <cassert>
#include <iostream>
#include "../scoremanager.hpp"
#include "../contract.hpp"
#include "../player.hpp"

void test_defaultConstructor() {
    ScoreManager sm;
    assert(sm.getScore(Team::NorthSouth) == 0);
    assert(sm.getScore(Team::EastWest) == 0);
    assert(sm.getTricksWon(Team::NorthSouth) == 0);
    assert(sm.getTricksWon(Team::EastWest) == 0);
    std::cout << "ScoreManager default constructor: OK\n";
}

void test_recordTrickWon() {
    ScoreManager sm;
    sm.recordTrickWon(Team::NorthSouth);
    sm.recordTrickWon(Team::NorthSouth);
    sm.recordTrickWon(Team::EastWest);
    assert(sm.getTricksWon(Team::NorthSouth) == 2);
    assert(sm.getTricksWon(Team::EastWest) == 1);
    std::cout << "ScoreManager::recordTrickWon: OK\n";
}

void test_contractMadeTrue() {
    Player d1(Position::North), d2(Position::South);
    Contract c(1, Suit::NoTrump, Team::NorthSouth, &d1, &d2); // needs 7
    ScoreManager sm;
    for (int i = 0; i < 7; ++i) sm.recordTrickWon(Team::NorthSouth);
    assert(sm.contractMade(c));
    std::cout << "ScoreManager::contractMade (exact tricks -> true): OK\n";
}

void test_contractMadeFalse() {
    Player d1(Position::North), d2(Position::South);
    Contract c(3, Suit::NoTrump, Team::NorthSouth, &d1, &d2); // needs 9
    ScoreManager sm;
    for (int i = 0; i < 8; ++i) sm.recordTrickWon(Team::NorthSouth);
    assert(!sm.contractMade(c));
    std::cout << "ScoreManager::contractMade (short of required -> false): OK\n";
}

void test_awardPointsPartScore() {
    // 1C made exactly: trickScore = 1*20 = 20 (< 100 -> part-score bonus 50).
    Player d1(Position::North), d2(Position::South);
    Contract c(1, Suit::Clubs, Team::NorthSouth, &d1, &d2); // needs 7
    ScoreManager sm;
    for (int i = 0; i < 7; ++i) sm.recordTrickWon(Team::NorthSouth);
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 70); // 20 + 50
    assert(sm.getScore(Team::EastWest) == 0);
    std::cout << "ScoreManager::awardPoints (part-score contract): OK\n";
}

void test_awardPointsGameBonus() {
    // 3NT made exactly: trickScore = 40 + 2*30 = 100 (>=100 -> game bonus 300).
    Player d1(Position::North), d2(Position::South);
    Contract c(3, Suit::NoTrump, Team::NorthSouth, &d1, &d2); // needs 9
    ScoreManager sm;
    for (int i = 0; i < 9; ++i) sm.recordTrickWon(Team::NorthSouth);
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 400); // 100 + 300
    std::cout << "ScoreManager::awardPoints (game-bonus contract): OK\n";
}

void test_awardPointsOvertricks() {
    // 1C bid (needs 7), team wins 8 (1 overtrick). Clubs overtrick = 20.
    Player d1(Position::East), d2(Position::West);
    Contract c(1, Suit::Clubs, Team::EastWest, &d1, &d2);
    ScoreManager sm;
    for (int i = 0; i < 8; ++i) sm.recordTrickWon(Team::EastWest);
    sm.awardPoints(c);
    // trickScore 20 + part-score bonus 50 + 1 overtrick * 20 = 90.
    assert(sm.getScore(Team::EastWest) == 90);
    std::cout << "ScoreManager::awardPoints (overtricks): OK\n";
}

void test_awardPointsSlamBonus() {
    // 6NT made exactly: trickScore = 40 + 5*30 = 190, game bonus 300,
    // small slam bonus 500.
    Player d1(Position::North), d2(Position::South);
    Contract c(6, Suit::NoTrump, Team::NorthSouth, &d1, &d2); // needs 12
    ScoreManager sm;
    for (int i = 0; i < 12; ++i) sm.recordTrickWon(Team::NorthSouth);
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 990); // 190 + 300 + 500
    std::cout << "ScoreManager::awardPoints (small slam bonus): OK\n";
}

void test_awardPointsGrandSlamBonus() {
    // 7NT made exactly: trickScore = 40 + 6*30 = 220, game bonus 300,
    // grand slam bonus 1000.
    Player d1(Position::East), d2(Position::West);
    Contract c(7, Suit::NoTrump, Team::EastWest, &d1, &d2); // needs 13
    ScoreManager sm;
    for (int i = 0; i < 13; ++i) sm.recordTrickWon(Team::EastWest);
    sm.awardPoints(c);
    assert(sm.getScore(Team::EastWest) == 1520); // 220 + 300 + 1000
    std::cout << "ScoreManager::awardPoints (grand slam bonus): OK\n";
}

void test_awardPointsUndertrickPenalty() {
    // 3NT (needs 9), declaring team only wins 7 -> 2 undertricks,
    // penalty 2*50 = 100 goes to the DEFENDING team.
    Player d1(Position::North), d2(Position::South);
    Contract c(3, Suit::NoTrump, Team::NorthSouth, &d1, &d2);
    ScoreManager sm;
    for (int i = 0; i < 7; ++i) sm.recordTrickWon(Team::NorthSouth);
    for (int i = 0; i < 6; ++i) sm.recordTrickWon(Team::EastWest);
    assert(!sm.contractMade(c));
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 0);   // declarer's team gets nothing
    assert(sm.getScore(Team::EastWest) == 100);    // defenders get the penalty
    std::cout << "ScoreManager::awardPoints (undertrick penalty to defenders): OK\n";
}

void test_getScore() {
    Player d1(Position::North), d2(Position::South);
    Contract c(1, Suit::Clubs, Team::NorthSouth, &d1, &d2);
    ScoreManager sm;
    for (int i = 0; i < 7; ++i) sm.recordTrickWon(Team::NorthSouth);
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 70);
    assert(sm.getScore(Team::EastWest) == 0);
    std::cout << "ScoreManager::getScore: OK\n";
}

void test_getTricksWon() {
    ScoreManager sm;
    sm.recordTrickWon(Team::EastWest);
    sm.recordTrickWon(Team::EastWest);
    sm.recordTrickWon(Team::EastWest);
    assert(sm.getTricksWon(Team::EastWest) == 3);
    assert(sm.getTricksWon(Team::NorthSouth) == 0);
    std::cout << "ScoreManager::getTricksWon: OK\n";
}

void test_display() {
    ScoreManager sm;
    sm.recordTrickWon(Team::NorthSouth);
    // No return value; confirm it runs without crashing.
    sm.display();
    std::cout << "ScoreManager::display (ran without crash): OK\n";
}

void test_reset() {
    Player d1(Position::North), d2(Position::South);
    Contract c(1, Suit::Clubs, Team::NorthSouth, &d1, &d2);
    ScoreManager sm;
    for (int i = 0; i < 7; ++i) sm.recordTrickWon(Team::NorthSouth);
    sm.awardPoints(c);
    assert(sm.getScore(Team::NorthSouth) == 70);
    sm.reset();
    // Tricks clear, but score persists across hands.
    assert(sm.getTricksWon(Team::NorthSouth) == 0);
    assert(sm.getTricksWon(Team::EastWest) == 0);
    assert(sm.getScore(Team::NorthSouth) == 70);
    std::cout << "ScoreManager::reset (clears tricks, keeps score): OK\n";
}

int main() {
    test_defaultConstructor();
    test_recordTrickWon();
    test_contractMadeTrue();
    test_contractMadeFalse();
    test_awardPointsPartScore();
    test_awardPointsGameBonus();
    test_awardPointsOvertricks();
    test_awardPointsSlamBonus();
    test_awardPointsGrandSlamBonus();
    test_awardPointsUndertrickPenalty();
    test_getScore();
    test_getTricksWon();
    test_display();
    test_reset();
    std::cout << "\nAll ScoreManager tests passed.\n";
    return 0;
}
