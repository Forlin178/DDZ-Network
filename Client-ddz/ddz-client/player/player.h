#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include "cards.h"

class Player : public QObject
{
    Q_OBJECT
public:
    enum Role {Lord, Farmer};
    enum Sex {Man, Woman};
    enum Direction {Left, Right};       // 人物图形的朝向
    enum Type {Robot, User, Unknow};

    explicit Player(QObject *parent = nullptr);
    explicit Player(QString name, QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &newName);

    Role role() const;
    void setRole(Role newRole);

    Sex sex() const;
    void setSex(Sex newSex);

    Direction direction() const;
    void setDirection(Direction newDirection);

    Type type() const;
    void setType(Type newType);

    int score() const;
    void setScore(int newScore);

    bool isWin() const;
    void setIsWin(bool newIsWin);

    Player *prev() const;
    void setPrev(Player *newPrev);

    Player *next() const;
    void setNext(Player *newNext);

    Cards cards() const;

    Cards pendCards() const;

    Player *pendPlayer() const;

    void grabLoadBet(int point);                            // 叫地主/抢地主
    void storeDispatchCard(Card &card);                     // 存储卡牌(发牌时使用)
    void storeDispatchCard(Cards &cards);                   // 存储卡牌(重载)
    void clearCards();                                      // 清空手牌
    void playHand(Cards &cards);                            // 出牌
    void setPendingInfo(Player *player, Cards &cards);      // 记录场上最大牌及出牌者(没有就随便出)
    void storePendingInfo(Player *player, const Cards &cards);

    // 玩家和机器人不同的处理逻辑
    virtual void prepareCallLord() = 0;                     // 叫/抢地主
    virtual void preparePlayHand() = 0;                     // 出牌

signals:
    void notifyGrabLordBet(Player *player, int bet);
    void notifyPlayHand(Player *player, const Cards &cards);
    void notifyPickCards(Player *player, const Cards &cards);

protected:
    int score_{ 0 };
    bool isWin_{ false };

    QString name_;
    Role role_;
    Sex sex_;
    Direction direction_;
    Type type_;

    Player *prev_;          // 上家
    Player *next_;          // 下家
    Cards cards_;           // 手牌

    Cards pendCards_;       // 记录上个出牌
    Player *pendPlayer_{nullptr};    // 记录上个出牌的玩家
};

#endif // PLAYER_H
