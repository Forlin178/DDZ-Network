#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{}

Player::Player(QString name, QObject *parent)
    : Player(parent)
{
    name_ = name;
}

QString Player::name() const
{
    return name_;
}

void Player::setName(const QString &newName)
{
    name_ = newName;
}

Player::Role Player::role() const
{
    return role_;
}

void Player::setRole(Role newRole)
{
    role_ = newRole;
}

Player::Sex Player::sex() const
{
    return sex_;
}

void Player::setSex(Sex newSex)
{
    sex_ = newSex;
}

Player::Direction Player::direction() const
{
    return direction_;
}

void Player::setDirection(Direction newDirection)
{
    direction_ = newDirection;
}

Player::Type Player::type() const
{
    return type_;
}

void Player::setType(Type newType)
{
    type_ = newType;
}

int Player::score() const
{
    return score_;
}

void Player::setScore(int newScore)
{
    score_ = newScore;
}

bool Player::isWin() const
{
    return isWin_;
}

void Player::setIsWin(bool newIsWin)
{
    isWin_ = newIsWin;
}

Player *Player::prev() const
{
    return prev_;
}

void Player::setPrev(Player *newPrev)
{
    prev_ = newPrev;
}

Player *Player::next() const
{
    return next_;
}

void Player::setNext(Player *newNext)
{
    next_ = newNext;
}

void Player::grabLoadBet(int point)
{
    emit notifyGrabLordBet(this, point);
}

void Player::storeDispatchCard(Card &card)
{
    cards_ << card;
    Cards cards;
    cards << card;
    emit notifyPickCards(this, cards);
}

void Player::storeDispatchCard(Cards &cards)
{
    cards_ << cards;
    emit notifyPickCards(this, cards);
}

void Player::clearCards()
{
    cards_.clear();
}

void Player::playHand(Cards &cards)
{
    cards_.remove(cards);
    emit notifyPlayHand(this, cards);
}

void Player::setPendingInfo(Player *player, Cards &cards)
{
    pendCards_ = cards;
    pendPlayer_ = player;
}

void Player::storePendingInfo(Player *player, const Cards &cards)
{
    pendCards_ = cards;
    pendPlayer_ = player;
}

Cards Player::pendCards() const
{
    return pendCards_;
}

Player *Player::pendPlayer() const
{
    return pendPlayer_;
}

Cards Player::cards() const
{
    return cards_;
}
