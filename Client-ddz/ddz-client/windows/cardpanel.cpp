#include "cardpanel.h"
#include <QPainter>
#include <QMouseEvent>

CardPanel::CardPanel(QWidget *parent)
    : QWidget{parent}
{
    isFront_ = true;
}

void CardPanel::clicked()
{
    emit cardSelected(Qt::LeftButton);
}

void CardPanel::setImage(QPixmap &front, QPixmap &back)
{
    front_ = front;
    back_ = back;
    setFixedSize(front_.size());    // 让窗口变成和图片一样大小
    update();                       // 手动让窗口重新绘制
}

QPixmap CardPanel::getImage()
{
    return front_;
}

Card CardPanel::getCard() const
{
    return card_;
}

void CardPanel::setCard(const Card &newCard)
{
    card_ = newCard;
}

void CardPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (isFront_) {
        painter.drawPixmap(rect(), front_);     // 让图片贴合这窗口自己的rect
    } else {
        painter.drawPixmap(rect(), back_);
    }
}

void CardPanel::mousePressEvent(QMouseEvent *event)
{
    emit cardSelected(event->button());
}
