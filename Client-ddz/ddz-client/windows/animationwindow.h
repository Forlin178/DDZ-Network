#ifndef ANIMATIONWINDOW_H
#define ANIMATIONWINDOW_H

#include <QWidget>

class AnimationWindow : public QWidget
{
    Q_OBJECT
public:
    enum Type{Sequence, Pair};

    explicit AnimationWindow(QWidget *parent = nullptr);

    void showBetScore(int bet);             // 显示下注分数
    void showSequence(Type type);           // 特效: 连对/顺子
    void showJokerBomb();                   // 特效: 王炸
    void showBomb();                        // 特效: 炸弹
    void showPlane();                       // 特效: 飞机

signals:

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    QPixmap image_;
    int index_;
    int x_;
};

#endif // ANIMATIONWINDOW_H
