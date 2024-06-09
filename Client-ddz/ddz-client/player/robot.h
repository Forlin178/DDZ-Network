#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include "player.h"

class Robot : public Player
{
    Q_OBJECT
public:
    using Player::Player;
    explicit Robot(QObject *parent = nullptr);

    // Player interface
public:
    virtual void prepareCallLord() override;
    virtual void preparePlayHand() override;

    void thinkCallLord();               // 考虑叫地主
    void thinkPlayHand();               // 考虑出牌
};

#endif // ROBOT_H
