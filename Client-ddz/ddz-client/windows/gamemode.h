#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <QDialog>

namespace Ui {
class GameMode;
}

class GameMode : public QDialog
{
    Q_OBJECT

public:
    explicit GameMode(QWidget *parent = nullptr);
    ~GameMode();

private:
    Ui::GameMode *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};


#endif // GAMEMODE_H