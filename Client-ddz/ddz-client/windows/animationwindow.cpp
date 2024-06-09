#include "animationwindow.h"
#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget *parent)
    : QWidget{parent}
{
    x_ = 0;
}

void AnimationWindow::showBetScore(int bet)
{
    x_ = 0;
    if (bet == 1) {
        image_.load(QString(":/image/images/score1.png"));
    }
    else if (bet == 2) {
        image_.load(QString(":/image/images/score2.png"));
    }
    else if (bet == 3) {
        image_.load(QString(":/image/images/score3.png"));
    }
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);     // 持续个2s就隐藏
}

void AnimationWindow::showSequence(Type type)
{
    x_ = 0;
    QString name = type == Sequence ? ":/image/images/shunzi.png" : ":/image/images/liandui.png";
    image_.load(name);
    update();
    QTimer::singleShot(1500, this, &AnimationWindow::hide);     // 持续个2s就隐藏
}

void AnimationWindow::showJokerBomb()
{
    x_ = 0;
    index_ = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        index_++;
        if (index_ > 8) {
            timer->stop();
            timer->deleteLater();
            index_ = 8;
            hide();
        }
        QString name = QString(":/image/images/joker_bomb_%1.png").arg(index_);
        image_.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showBomb()
{
    x_ = 0;
    index_ = 0;
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        index_++;
        if (index_ > 12) {
            timer->stop();
            timer->deleteLater();
            index_ = 12;
            hide();
        }
        QString name = QString(":/image/images/bomb_%1.png").arg(index_);
        image_.load(name);
        update();
    });
    timer->start(60);
}

void AnimationWindow::showPlane()
{
    x_ = width();
    setFixedHeight(75);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        x_ -= 5;
        if (x_ <= 0) {
            timer->stop();
            timer->deleteLater();
            x_ = 0;
            hide();
        }
        image_.load(QString(":/image/images/plane_5.png"));
        update();
    });
    timer->start(15);
}

void AnimationWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(x_, 0, image_.width(), image_.height(), image_);
}
