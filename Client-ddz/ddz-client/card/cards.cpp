#include "cards.h"
#include <QRandomGenerator>
#include <algorithm>
#include <datamanager.h>

Cards::Cards() {}

void Cards::add(const Card &card)
{
    cards_.append(card);
}

void Cards::add(const Cards &cards)
{
    cards_.append(cards.cards_);
}

void Cards::add(const QVector<Cards> &cardsArray)
{
    for (const auto& iter : cardsArray) {
        add(iter);
    }
}

void Cards::remove(const Card &card)
{
    cards_.removeOne(card);
}

void Cards::remove(const Cards &cards)
{
    // cards_.subtract(cards.cards_);
    for (const auto& iter : cards.cards_) {
        cards_.removeOne(iter);
    }
}

void Cards::remove(const QVector<Cards> &cardsArray)
{
    for (const auto& iter : cardsArray) {
        remove(iter);
    }
}

Card::CardPoint Cards::maxPoint()
{
    Card::CardPoint max = Card::Point_begin;
    for (const auto& iter : cards_) {
        max = iter.point() > max ? iter.point() : max;
    }
    return max;
}

Card::CardPoint Cards::minPoint()
{
    Card::CardPoint min = Card::Point_end;
    for (const auto& iter : cards_) {
        min = iter.point() < min ? iter.point() : min;
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point)
{
    int count = 0;
    for (const auto& iter: cards_) {
        if (iter.point() == point)
            count++;
    }
    return count;
}

bool Cards::contains(const Card &card)
{
    return cards_.contains(card);
}

bool Cards::contains(const Cards &cards)
{
    // return cards_.contains(cards.cards_);
    for (const auto& iter : cards.cards_) {
        if (!cards_.contains(iter))
            return false;
    }
    return true;
}

Card Cards::takeRandomCard()
{
    Card card;
    if (DataManager::getInstance()->mode() == DataManager::Single) {
        int num = QRandomGenerator::global()->bounded(cards_.size());
        auto iter = cards_.begin();
        for (int i=0; i<num; i++) {
            iter++;
        }
        cards_.erase(iter);
        card = *iter;
    } else {
        card = cards_.takeFirst();
    }
    return card;
}

CardList Cards::toCardList(SortType type) const
{
    CardList list;
    for (const auto& iter : cards_) {
        list << iter;
    }
    if (type == SortType::ASC) {
        std::sort(list.begin(), list.end(), [](const Card& card1, const Card& card2){
            if (card1.point() == card2.point())
                return card1.suit() < card2.suit();
            return card1.point() < card2.point();
        });
    }
    else if (type == SortType::DESC) {
        std::sort(list.begin(), list.end(), [](const Card& card1, const Card& card2){
            if (card1.point() == card2.point())
                return card1.suit() > card2.suit();
            return card1.point() > card2.point();
        });
    }
    return list;
}

Cards &Cards::operator <<(const Card &card)
{
    add(card);
    return *this;
}

Cards &Cards::operator <<(const Cards &cards)
{
    add(cards);
    return *this;
}
