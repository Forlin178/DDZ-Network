#ifndef CARDS_H
#define CARDS_H

#include "card.h"
#include <QSet>

class Cards
{
public:
    enum SortType {ASC, DESC, NoSort};

    Cards();

    // 添加卡牌
    void add(const Card& card);
    void add(const Cards& cards);
    void add(const QVector<Cards>& cardsArray);
    // 删除卡牌
    void remove(const Card& card);
    void remove(const Cards& cards);
    void remove(const QVector<Cards>& cardsArray);

    int cardCount() { return cards_.count(); }      // 手牌数量
    bool isEmpty() const { return cards_.empty(); } // 判空
    void clear() { cards_.clear(); }                // 清空

    Card::CardPoint maxPoint();                     // 最大点数
    Card::CardPoint minPoint();                     // 最小点数
    int pointCount(Card::CardPoint point);          // 指定点数的手牌数量
    bool contains(const Card& card);                // 某牌是否在手中
    bool contains(const Cards& cards);              // 某些牌是否在手中

    Card takeRandomCard();                          // 随机取一张牌(发牌用)

    CardList toCardList(SortType type = DESC) const;      // 将无序的set转化成有序的vector

    Cards &operator <<(const Card& card);
    Cards &operator <<(const Cards& cards); 

private:
    QList<Card> cards_;
    // QSet<Card> cards_;
};

#endif // CARDS_H
