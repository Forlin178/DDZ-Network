#include "gamepanel.h"
#include "loading.h"
#include <QPainter>
#include <QTimer>

Loading::Loading(QWidget *parent)
    : QWidget{parent}
{
    backGround_.load(":/image/images/loading.png");
    setFixedSize(backGround_.size());

    setWindowFlag(Qt::FramelessWindowHint);             // 无边框
    setAttribute(Qt::WA_TranslucentBackground);         // 透明背景

    QTimer *timer = new QTimer(this);
    QPixmap temp(":/image/images/progress.png");
    connect(timer, &QTimer::timeout, this, [=](){
        progress_ = temp.copy(0, 0, length_, 41);
        length_ += 5;
        update();
        if (length_ >= 473) {
            length_ = 473;
            timer->stop();
            timer->deleteLater();

            GamePanel *panel = new GamePanel();    // parent设为nullptr 因为loading窗口要关闭，不能让panel跟着关闭
            panel->show();
            this->close();
        }
    });

    timer->start(20);
}

void Loading::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backGround_);
    painter.drawPixmap(62, 417, progress_.width(), progress_.height(), progress_);
}
