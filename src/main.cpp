#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "common.hpp"
#include "deck.hpp"
#include "player.hpp"
#include "bid.hpp"
#include "biddingmanager.hpp"
#include "contract.hpp"
#include "trick.hpp"
#include "scoremanager.hpp"

void clearInvalidInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void waitForPlayer(const std::string& playerName)
{
    std::cout << "\nPass the computer to " << playerName << ".\n";
    std::cout << "Press Enter when ready...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void hidePreviousScreen()
{
    for (int i = 0; i < 35; i++)
    {
        std::cout << '\n';
    }
}

std::string positionToString(Position position)
{
    switch (position)
    {
        case Position::North: return "North";
        case Position::East: return "East";
        case Position::South: return "South";
        case Position::West: return "West";
        default: return "Unknown";
    }
}

std::string teamToString(Team team)
{
    if (team == Team::NorthSouth)
    {
        return "North/South";
    }
    return "East/West";
}

std::string cardToString(const Card& card)
{
    return rankToString(card.getRank()) + " of " + suitToString(card.getSuit());
}

Suit getSuitFromChoice(int choice)
{
    switch (choice)
    {
        case 1: return Suit::Clubs;
        case 2: return Suit::Diamonds;
        case 3: return Suit::Hearts;
        case 4: return Suit::Spades;
        case 5: return Suit::NoTrump;
        default: return Suit::NoTrump;
    }
}

void displayHighestBid(const BiddingManager& biddingManager)
{
    if (biddingManager.getHighestBidder() == nullptr)
    {
        std::cout << "Current highest bid: No bid yet\n";
        return;
    }

    std::cout << "Current highest bid: "
              << biddingManager.getHighestBid().toString()
              << " by "
              << biddingManager.getHighestBidder()->getName()
              << '\n';
}

Bid getBidFromPlayer(Player& player, const BiddingManager& biddingManager)
{
    while (true)
    {
        std::cout << "\n========================================\n";
        std::cout << player.getName() << " (" << positionToString(player.getPosition())
                  << "), it is your turn to bid.\n";
        std::cout << "========================================\n";

        displayHighestBid(biddingManager);
        std::cout << "\nYour hand:\n";
        player.displayHand();

        std::cout << "\nBidding choices:\n";
        std::cout << "1. Make a normal bid\n";
        std::cout << "2. Pass\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInvalidInput();
            std::cout << "Invalid input. Please enter a number from 1 to 2.\n";
            continue;
        }

        if (choice == 2) return Bid::makePass();

        if (choice != 1)
        {
            std::cout << "Invalid choice. Please enter a number from 1 to 2.\n";
            continue;
        }

        int level;
        int suitChoice;

        std::cout << "\nEnter bid level from 1 to 7: ";
        std::cin >> level;

        if (std::cin.fail())
        {
            clearInvalidInput();
            std::cout << "Invalid level.\n";
            continue;
        }

        std::cout << "\nChoose a suit:\n";
        std::cout << "1. Clubs\n";
        std::cout << "2. Diamonds\n";
        std::cout << "3. Hearts\n";
        std::cout << "4. Spades\n";
        std::cout << "5. No Trump\n";
        std::cout << "Enter your suit choice: ";
        std::cin >> suitChoice;

        if (std::cin.fail())
        {
            clearInvalidInput();
            std::cout << "Invalid suit choice.\n";
            continue;
        }

        if (level < 1 || level > 7)
        {
            std::cout << "The bid level must be from 1 to 7.\n";
            continue;
        }

        if (suitChoice < 1 || suitChoice > 5)
        {
            std::cout << "The suit choice must be from 1 to 5.\n";
            continue;
        }

        return Bid(level, getSuitFromChoice(suitChoice));
    }
}

Contract runBidding(std::vector<Player*>& players, Position dealer)
{
    BiddingManager biddingManager;
    biddingManager.setPlayers(players);
    int dealerIndex = 0;

    for (int i = 0; i < static_cast<int>(players.size()); i++)
    {
        if (players[i]->getPosition() == dealer)
        {
            dealerIndex = i;
            break;
        }
    }

    int currentIndex = dealerIndex;

    std::cout << "\n========================================\n";
    std::cout << "             BIDDING PHASE\n";
    std::cout << "========================================\n";
    std::cout << "Dealer: " << positionToString(dealer) << '\n';

    while (!biddingManager.isBiddingComplete())
    {
        Player* currentPlayer = players[currentIndex];
        hidePreviousScreen();
        waitForPlayer(currentPlayer->getName());

        Bid bid = getBidFromPlayer(*currentPlayer, biddingManager);

        if (biddingManager.submitBid(*currentPlayer, bid))
        {
            std::cout << "\nAccepted bid: " << currentPlayer->getName()
                      << " chose " << bid.toString() << '\n';
            currentIndex = (currentIndex + 1) % static_cast<int>(players.size());
        }
        else
        {
            std::cout << "\nThat bid is not legal.\n";
            if (bid.isNormal())
            {
                std::cout << "A normal bid must be higher than the current bid.\n";
            }
            std::cout << "Please try again.\n";
        }
    }

    hidePreviousScreen();
    Contract contract = biddingManager.getContract();

    if (contract.getDeclarer() == nullptr)
    {
        std::cout << "\nAll four players passed.\n";
        std::cout << "The deal has no contract.\n";
    }
    else
    {
        std::cout << "\n========================================\n";
        std::cout << "             FINAL CONTRACT\n";
        std::cout << "========================================\n";
        contract.display();
    }
    return contract;
}

int getLegalCardChoice(Player& player, Suit leadSuit)
{
    while (true)
    {
        std::cout << "\nYour hand:\n";
        player.displayHand();

        if (leadSuit == Suit::NoTrump)
        {
            std::cout << "\nYou are leading this trick.\n";
        }
        else
        {
            std::cout << "\nLead suit: " << suitToString(leadSuit) << '\n';
        }

        std::cout << "Enter the index of the card you want to play: ";
        int cardIndex;
        std::cin >> cardIndex;

        if (std::cin.fail())
        {
            clearInvalidInput();
            std::cout << "Invalid input. Enter one of the displayed indexes.\n";
            continue;
        }

        Hand hand = player.getHand();
        if (cardIndex < 0 || cardIndex >= hand.size())
        {
            std::cout << "That card index does not exist.\n";
            continue;
        }

        if (!hand.isValidPlay(cardIndex, leadSuit))
        {
            std::cout << "That play is not legal because you must follow "
                      << suitToString(leadSuit) << ".\n";
            continue;
        }
        return cardIndex;
    }
}

void displayCardsInTrick(const Trick& trick)
{
    const std::vector<std::pair<Player*, Card> >& plays = trick.getPlays();
    std::cout << "\nCards played in this trick:\n";

    if (plays.empty())
    {
        std::cout << "No cards have been played yet.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(plays.size()); i++)
    {
        std::cout << plays[i].first->getName() << ": "
                  << cardToString(plays[i].second) << '\n';
    }
}

Position playOneTrick(std::vector<Player*>& players, Position leadPosition,
                      Suit trumpSuit, ScoreManager& scoreManager, int trickNumber)
{
    Trick trick(trumpSuit);
    Position currentPosition = leadPosition;

    std::cout << "\n========================================\n";
    std::cout << "               TRICK " << trickNumber << '\n';
    std::cout << "========================================\n";
    std::cout << "Leader: " << positionToString(leadPosition) << '\n';

    for (int turn = 0; turn < 4; turn++)
    {
        Player* currentPlayer = nullptr;

        for (int i = 0; i < static_cast<int>(players.size()); i++)
        {
            if (players[i]->getPosition() == currentPosition)
            {
                currentPlayer = players[i];
                break;
            }
        }

        if (currentPlayer != nullptr)
        {
            hidePreviousScreen();
            waitForPlayer(currentPlayer->getName());

            std::cout << "\n========================================\n";
            std::cout << currentPlayer->getName() << "'s turn\n";
            std::cout << "Position: " << positionToString(currentPlayer->getPosition()) << '\n';
            std::cout << "========================================\n";

            displayCardsInTrick(trick);
            int cardIndex = getLegalCardChoice(*currentPlayer, trick.getLeadSuit());
            Card playedCard = currentPlayer->playCard(cardIndex);
            trick.addPlay(currentPlayer, playedCard);

            std::cout << "\n" << currentPlayer->getName() << " played "
                      << cardToString(playedCard) << ".\n";
        }
        currentPosition = nextPosition(currentPosition);
    }

    hidePreviousScreen();
    std::cout << "\n========================================\n";
    std::cout << "           TRICK " << trickNumber << " RESULT\n";
    std::cout << "========================================\n";
    displayCardsInTrick(trick);

    Player* winner = trick.determineWinner();
    if (winner != nullptr)
    {
        scoreManager.recordTrickWon(winner->getTeam());
        std::cout << "\n" << winner->getName() << " wins the trick for "
                  << teamToString(winner->getTeam()) << ".\n";
        std::cout << "\nCurrent trick totals:\n";
        std::cout << "North/South: "
                  << scoreManager.getTricksWon(Team::NorthSouth) << '\n';
        std::cout << "East/West: "
                  << scoreManager.getTricksWon(Team::EastWest) << '\n';
        std::cout << "\nPress Enter to begin the next trick...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return winner->getPosition();
    }
    return leadPosition;
}

void playContract(std::vector<Player*>& players, const Contract& contract,
                  ScoreManager& scoreManager)
{
    if (contract.getDeclarer() == nullptr)
    {
        return;
    }

    Position leader = nextPosition(contract.getDeclarer()->getPosition());
    std::cout << "\nThe opening leader is " << positionToString(leader) << ".\n";
    std::cout << "Trump suit: " << suitToString(contract.getTrumpSuit()) << '\n';

    for (int trickNumber = 1; trickNumber <= 13; trickNumber++)
    {
        leader = playOneTrick(players, leader, contract.getTrumpSuit(),
                              scoreManager, trickNumber);
    }
}

void displayDealResult(const Contract& contract, ScoreManager& scoreManager)
{
    std::cout << "\n========================================\n";
    std::cout << "               DEAL RESULT\n";
    std::cout << "========================================\n";
    std::cout << "North/South tricks: "
              << scoreManager.getTricksWon(Team::NorthSouth) << '\n';
    std::cout << "East/West tricks: "
              << scoreManager.getTricksWon(Team::EastWest) << '\n';

    if (contract.getDeclarer() == nullptr)
    {
        std::cout << "\nPassed-out deal. No points are awarded.\n";
        return;
    }

    Team declaringTeam = contract.getDeclaringTeam();
    bool madeContract = scoreManager.contractMade(contract);
    int declaringTricks = scoreManager.getTricksWon(declaringTeam);

    std::cout << "\nDeclaring team: " << teamToString(declaringTeam) << '\n';
    std::cout << "Required tricks: " << contract.getRequiredTricks() << '\n';
    std::cout << "Tricks won by declaring team: " << declaringTricks << '\n';
    std::cout << "Result: "
              << (madeContract ? "Contract made.\n" : "Contract defeated.\n");

    scoreManager.awardPoints(contract);
    std::cout << "\nMatch score:\n";
    std::cout << "North/South: " << scoreManager.getScore(Team::NorthSouth) << '\n';
    std::cout << "East/West: " << scoreManager.getScore(Team::EastWest) << '\n';
}

void dealCards(Deck& deck, std::vector<Player*>& players)
{
    deck.createDeck(); //resets deck 
    deck.shuffle();

    for (int cardNumber = 0; cardNumber < 13; cardNumber++)
    {
        for (int playerIndex = 0;
             playerIndex < static_cast<int>(players.size()); playerIndex++)
        {
            players[playerIndex]->receiveCard(deck.dealCard());
        }
    }
}

int main()
{
    std::cout << "========================================\n";
    std::cout << "       FOUR-PLAYER CONTRACT BRIDGE\n";
    std::cout << "========================================\n";
    std::cout << "This game is played by four people on one computer.\n";
    std::cout << "Players should pass the computer between turns.\n\n";

    std::string northName;
    std::string eastName;
    std::string southName;
    std::string westName;

    std::cout << "Enter the North player's name: ";
    std::getline(std::cin, northName);
    std::cout << "Enter the East player's name: ";
    std::getline(std::cin, eastName);
    std::cout << "Enter the South player's name: ";
    std::getline(std::cin, southName);
    std::cout << "Enter the West player's name: ";
    std::getline(std::cin, westName);

    if (northName.empty()) northName = "North";
    if (eastName.empty()) eastName = "East";
    if (southName.empty()) southName = "South";
    if (westName.empty()) westName = "West";

    ScoreManager scoreManager;
    Position dealer = Position::North;
    int boardNumber = 1;
    char playAgain = 'Y';

    while (playAgain == 'Y' || playAgain == 'y')
    {
        Player northPlayer(northName, Position::North, Team::NorthSouth);
        Player eastPlayer(eastName, Position::East, Team::EastWest);
        Player southPlayer(southName, Position::South, Team::NorthSouth);
        Player westPlayer(westName, Position::West, Team::EastWest);

        std::vector<Player*> players;
        players.push_back(&northPlayer);
        players.push_back(&eastPlayer);
        players.push_back(&southPlayer);
        players.push_back(&westPlayer);

        Deck deck;
        dealCards(deck, players);
        scoreManager.reset();

        std::cout << "\n========================================\n";
        std::cout << "                 BOARD " << boardNumber << '\n';
        std::cout << "========================================\n";
        std::cout << "Dealer: " << positionToString(dealer) << '\n';

        Contract contract = runBidding(players, dealer);

        if (contract.getDeclarer() != nullptr)
        {
            playContract(players, contract, scoreManager);
        }

        displayDealResult(contract, scoreManager);

        std::cout << "\n========================================\n";
        std::cout << "              TOTAL SCORE\n";
        std::cout << "========================================\n";
        std::cout << "North/South: " << scoreManager.getScore(Team::NorthSouth) << '\n';
        std::cout << "East/West: " << scoreManager.getScore(Team::EastWest) << '\n';

        std::cout << "\nPlay another deal? Enter Y or N: ";
        std::cin >> playAgain;

        if (playAgain == 'Y' || playAgain == 'y')
        {
            dealer = nextPosition(dealer);
            boardNumber++;
            clearInvalidInput();
        }
    }

    std::cout << "\n========================================\n";
    std::cout << "               FINAL SCORE\n";
    std::cout << "========================================\n";
    std::cout << "North/South: " << scoreManager.getScore(Team::NorthSouth) << '\n';
    std::cout << "East/West: " << scoreManager.getScore(Team::EastWest) << '\n';

    if (scoreManager.getScore(Team::NorthSouth) >
        scoreManager.getScore(Team::EastWest))
    {
        std::cout << "North/South wins the match!\n";
    }
    else if (scoreManager.getScore(Team::EastWest) >
             scoreManager.getScore(Team::NorthSouth))
    {
        std::cout << "East/West wins the match!\n";
    }
    else
    {
        std::cout << "The match is tied!\n";
    }

    std::cout << "\nGame finished.\n";
    return 0;
}
