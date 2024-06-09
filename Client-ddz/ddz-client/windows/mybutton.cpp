#include "mybutton.h"
#include <QMouseEvent>
#include <QPainter>

MyButton::MyButton(QWidget *parent)
    : QPushButton{parent}
{}

void MyButton::setImage(QString normal, QString hover, QString pressed)
{
    normal_ = normal;
    hover_ = hover;
    pressed_ = pressed;
    pixmap_.load(normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pixmap_.load(pressed_);
        update();
    }
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        pixmap_.load(normal_);
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEvent *event)
{
    pixmap_.load(hover_);
    update();
    QPushButton::enterEvent(event);
}

void MyButton::leaveEvent(QEvent *event)
{
    pixmap_.load(normal_);
    update();
    QPushButton::leaveEvent(event);
}

void MyButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(rect(), pixmap_);
}
