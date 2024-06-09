#include "countdown.h"
#include <QPainter>

CountDown::CountDown(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(70, 70);
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, this, [=](){
        count_--;
        if (count_ < 10 && count_ > 0) {        // 刷新剩余时间的图片
            clock_.load(QString(":/image/images/clock.png"));
            number_ = QPixmap(":/image/images/number.png").copy(count_*40, 0, 30, 42).scaled(20, 30);
            if (count_ == 5)
                emit notMuchTime();             // 音乐：提示用户时间不多
        }
        else if (count_ <= 0) {                 // 将两个图片置空 取消显示
            clock_ = QPixmap();
            number_ = QPixmap();
            timer_->stop();
            emit timeout();                     // 玩家剩余时间用完
        }
        update();
    });
}

void CountDown::showCountDown()
{
    count_ = 20;
    timer_->start(1000);    // 定时器出发间隔为1000ms = 1s
}

void CountDown::stopCountDown()
{
    timer_->stop();
    clock_ = QPixmap();
    number_ = QPixmap();
    update();
}

void CountDown::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), clock_);
    painter.drawPixmap(24, 24, number_.width(), number_.height(), number_);
}
