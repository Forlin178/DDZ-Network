#ifndef COUNTDOWN_H
#define COUNTDOWN_H

#include <QWidget>
#include <QTimer>

class CountDown : public QWidget
{
    Q_OBJECT
public:
    explicit CountDown(QWidget *parent = nullptr);

    void showCountDown();       // 显示玩家剩余时间
    void stopCountDown();       // 玩家回合结束后停止显示倒计时

signals:
    void notMuchTime();
    void timeout();

private:
    QPixmap clock_;     // 倒计时图片
    QPixmap number_;    // 剩余时间图片
    QTimer *timer_;
    int count_;         // 倒计时总时长

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};

#endif // COUNTDOWN_H
