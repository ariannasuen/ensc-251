#include "game.hpp"

Game::Game()
    : currentDealer_(Position::North),
      state_(GameState::Setup)
{
}

void Game::initialize()
{
    createPlayers();
    shuffleAndDeal();

    state_ = GameState::Bidding;
}

void Game::createPlayers()
{
    players_.clear();

    players_.push_back(Player(Position::North));
    players_.push_back(Player(Position::East));
    players_.push_back(Player(Position::South));
    players_.push_back(Player(Position::West));
}

void Game::shuffleAndDeal()
{
    deck_.reset();
    deck_.shuffle();

    // Example:
    // Each player receives 13 cards
    for (int i = 0; i < 13; i++)
    {
        for (auto& player : players_)
        {
            player.addCard(deck_.drawTopCard());
        }
    }
}

void Game::startBidding()
{
    state_ = GameState::Bidding;

    biddingManager_.startBidding(
        players_,
        currentDealer_
    );

    contract_ = biddingManager_.getContract();

    state_ = GameState::Playing;
}

void Game::startPlay()
{
    state_ = GameState::Playing;

    // You will likely replace this with a TrickManager/GamePlay class
    // later.
    controlTurnOrder();

    state_ = GameState::Scoring;
}

void Game::determineResult()
{
    state_ = GameState::Scoring;

    scoreManager_.calculateScore(
        contract_
    );

    state_ = GameState::Finished;
}

void Game::controlTurnOrder()
{
    Position current = nextPosition(currentDealer_);

    while (state_ == GameState::Playing)
    {
        // Find player whose turn it is
        for (auto& player : players_)
        {
            if (player.getPosition() == current)
            {
                // Placeholder:
                // player.playCard();
            }
        }

        current = nextPosition(current);

        // Temporary condition until trick/game logic exists
        break;
    }
}

void Game::endGame()
{
    state_ = GameState::Finished;
}

void Game::run()
{
    while (state_ != GameState::Finished)
    {
        switch (state_)
        {
            case GameState::Setup:
                initialize();
                break;

            case GameState::Bidding:
                startBidding();
                break;

            case GameState::Playing:
                startPlay();
                break;

            case GameState::Scoring:
                determineResult();
                break;

            case GameState::Finished:
                endGame();
                break;
        }
    }
}

GameState Game::getState() const
{
    return state_;
}
