#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    enum Panel {
        Start,
        PlayCard,
        PassOrPlay,
        CallLord,
        Empty
    };

    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    void initButton();                  // 初始化button
    void selectPanel(Panel type, int bet);       // page的切换

signals:
    void startGame();                   // 开始游戏
    void playHand();                    // 出牌
    void pass();                        // 要不起
    void betPoint(int bet);             // 叫地主

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
