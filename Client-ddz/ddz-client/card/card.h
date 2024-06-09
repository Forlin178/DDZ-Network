#ifndef CARD_H
#define CARD_H

#include <QVector>
#include <QDataStream>

class Card
{
public:
    friend QDataStream & operator<< (QDataStream& stream, const Card& card);
    friend QDataStream & operator>> (QDataStream& stream, Card& card);

    enum CardSuit{      // 花色
        Suit_begin,     // 为了方便
        Diamond,
        Club,
        Heart,
        Spade,
        Suit_end
    };

    enum CardPoint{     // 点数
        Point_begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ,        // 小王
        Card_BJ,        // 大王
        Point_end
    };

    Card();
    Card(CardPoint point, CardSuit suit);

    CardSuit suit() const;
    void setSuit(CardSuit newSuit);

    CardPoint point() const;
    void setPoint(CardPoint newPoint);

private:
    CardSuit suit_;
    CardPoint point_;
};

// 给QMap提供的
bool operator <(const Card &c1, const Card &c2);

// 以下两个函数是为了让QSet能使用自定义类
// 操作符== 重载
bool operator ==(const Card &c1, const Card &c2);

// 重写全局函数 qhash
uint qHash(const Card &card);

using CardList = QVector<Card>;

#endif // CARD_H
