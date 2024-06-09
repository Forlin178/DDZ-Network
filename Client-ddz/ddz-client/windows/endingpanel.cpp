#include "endingpanel.h"
#include <QPainter>

EndingPanel::EndingPanel(bool isLord, bool isWin, QWidget *parent)
    : QWidget{parent}
{
    backImage_.load(QString(":/image/images/gameover.png"));
    setFixedSize(backImage_.size());    // 将窗口的大小设置成图片大小

    title_ = new QLabel(this);
    if (isLord && isWin)
        title_->setPixmap(QPixmap(":/image/images/lord_win.png"));
    else if (isLord && !isWin)
        title_->setPixmap(QPixmap(":/image/images/lord_fail.png"));
    else if (!isLord && isWin)
        title_->setPixmap(QPixmap(":/image/images/farmer_win.png"));
    else if (!isLord && !isWin)
        title_->setPixmap(QPixmap(":/image/images/farmer_fail.png"));
    title_->move(125, 125);

    score_ = new ScorePanel(this);
    score_->move(75, 230);
    score_->setFixedSize(260, 160);
    score_->setFontSize(18);
    score_->setFontColor(Qt::red);

    continue_ = new QPushButton(this);
    continue_->move(84, 429);
    QString style = "QPushButton{border-image: url(:/image/images/button_normal.png)}"
                    "QPushButton:hover{border-image: url(:/image/images/button_hover.png)}"
                    "QPushButton:pressed{border-image: url(:/image/images/button_pressed.png)}";
    continue_->setStyleSheet(style);
    continue_->setFixedSize(231, 48);
    connect(continue_, &QPushButton::clicked, this, [=](){
        emit continueGame();
    });
}

void EndingPanel::setPlayerScore(int left, int right, int me)
{
    score_->setScore(left, right, me);
}

void EndingPanel::setPlayerName(QByteArrayList names)
{
    score_->setPlayerName(names[0], names[1], names[2]);
}

void EndingPanel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), backImage_);
}
