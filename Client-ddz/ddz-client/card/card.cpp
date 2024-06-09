#include "card.h"

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit)
    : point_(point), suit_(suit)
{}

Card::CardSuit Card::suit() const
{
    return suit_;
}

void Card::setSuit(CardSuit newSuit)
{
    suit_ = newSuit;
}

Card::CardPoint Card::point() const
{
    return point_;
}

void Card::setPoint(CardPoint newPoint)
{
    point_ = newPoint;
}

bool operator <(const Card &c1, const Card &c2) {
    if (c1.point() == c2.point())
        return c1.suit() < c2.suit();
    return c1.point() < c2.point();
}

bool operator ==(const Card &c1, const Card &c2) {
    return c1.point() == c2.point() && c1.suit() == c2.suit();
}

uint qHash(const Card &card) {
    return card.point() * 100 + card.suit();
}

QDataStream & operator<< (QDataStream& stream, const Card& card) {
    int suit = static_cast<int>(card.suit_);
    int point = static_cast<int>(card.point_);
    stream << suit << point;
    return stream;
}

QDataStream & operator>> (QDataStream& stream, Card& card) {
    int suit, point;
    stream >> suit >> point;
    card.setSuit(static_cast<Card::CardSuit>(suit));
    card.setPoint(static_cast<Card::CardPoint>(point));
    return stream;
}

