#include "robotplayhand.h"
#include <QThread>

RobotPlayHand::RobotPlayHand(Robot *player, QObject *parent)
    : QObject{parent}
{
    player_ = player;
    setAutoDelete(true);
}

void RobotPlayHand::run()
{
    QThread::msleep(1000);
    player_->thinkPlayHand();
}
