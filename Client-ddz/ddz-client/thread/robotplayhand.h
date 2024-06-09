#ifndef ROBOTPLAYHAND_H
#define ROBOTPLAYHAND_H

#include <QObject>
#include <QRunnable>
#include "robot.h"

class RobotPlayHand : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotPlayHand(Robot *player, QObject *parent = nullptr);

signals:

    // QThread interface
protected:
    virtual void run() override;

private:
    Robot *player_;
};

#endif // ROBOTPLAYHAND_H
