#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>
#include <QVector>
#include <QLabel>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget
{
    Q_OBJECT

public:
    explicit ScorePanel(QWidget *parent = nullptr);
    ~ScorePanel();

    void setPlayerName(QByteArray left, QByteArray mid, QByteArray right);  // 设置玩家名字
    void setScore(int left, int right, int user);       // 设置分数
    void setFontSize(int point);                        // 设置字体大小
    void setFontColor(Qt::GlobalColor color);                 // 设置字体颜色

private:
    Ui::ScorePanel *ui;
    QVector<QLabel*> list_;
};

#endif // SCOREPANEL_H
