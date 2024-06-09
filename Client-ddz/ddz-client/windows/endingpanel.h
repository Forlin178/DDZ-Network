#ifndef ENDINGPANEL_H
#define ENDINGPANEL_H

#include "scorepanel.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>

class EndingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit EndingPanel(bool isLord, bool isWin, QWidget *parent = nullptr);

    void setPlayerScore(int left, int right, int me);
    void setPlayerName(QByteArrayList names);

signals:
    void continueGame();

private:
    QPixmap backImage_;
    QLabel *title_;
    ScorePanel *score_;
    QPushButton *continue_;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
};


#endif // ENDINGPANEL_H
