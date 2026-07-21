#pragma once

#include <vector>
#include "deck.hpp"
#include "player.hpp"
#include "contract.hpp"
#include "biddingmanager.hpp"
#include "scoremanager.hpp"
#include "common.hpp"

enum class GameState {
    Setup,
    Bidding,
    Playing,
    Scoring,
    Finished
};

class Game {
public:
    Game();

    void initialize();
    void createPlayers();
    void shuffleAndDeal();
    void startBidding();
    void startPlay();
    void determineResult();
    void controlTurnOrder();
    void endGame();

    void run();   // drives the game through each state in order

    GameState getState() const;

private:
    Deck deck_;
    std::vector<Player> players_;
    Contract contract_;
    ScoreManager scoreManager_;
    BiddingManager biddingManager_;
    Position currentDealer_;
    GameState state_;
};
