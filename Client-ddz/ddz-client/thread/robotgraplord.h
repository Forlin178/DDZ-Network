#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include <QObject>
#include <QRunnable>
#include "robot.h"

class RobotGrapLord : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit RobotGrapLord(Robot *player, QObject *parent = nullptr);

signals:

    // QThread interface
protected:
    virtual void run() override;

private:
    Robot *player_;
};

#endif // ROBOTGRAPLORD_H
