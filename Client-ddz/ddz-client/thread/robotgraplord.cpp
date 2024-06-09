#include "robotgraplord.h"
#include <QThread>

RobotGrapLord::RobotGrapLord(Robot *player, QObject *parent)
    : QObject(parent)
{
    player_ = player;
    setAutoDelete(true);
}

void RobotGrapLord::run()
{
    QThread::msleep(1000);
    player_->thinkCallLord();
}
