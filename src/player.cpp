#include "player.hpp"

Player::Player()
{
    name = "";
    position = Position::NORTH;
    team = Team::NORTH_SOUTH;
}

Player::Player(const std::string& newName, Position newPosition, Team newTeam)
{
    name = newName;
    position = newPosition;
    team = newTeam;
}

Player::~Player()
{
    
}

std::string Player::getName() const
{
    return name;
}

Position Player::getPosition() const
{
    return position;
}

Team Player::getTeam() const
{
    return team;
}

Hand Player::getHand() const
{
    return hand;
}

void Player::receiveCard(const Card& card)
{
    hand.addCard(card);
}

Card Player::playCard(int index)
{
    Card playedCard = hand.getCard(index);
    hand.removeCard(index);
    return playedCard;
}

void Player::displayHand() const
{
    hand.displayHand();
}

bool Player::hasSuit(Suit suit) const
{
    return hand.hasSuit(suit);
}
