#include "scorepanel.h"
#include "ui_scorepanel.h"

ScorePanel::ScorePanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ScorePanel)
{
    ui->setupUi(this);
    list_ << ui->myName << ui->leftName << ui->rightName
          << ui->labscore_1 << ui->labscore_2 << ui->labscore_3
          << ui->labunit_1 << ui->labunit_2 << ui->labunit_3;
}

ScorePanel::~ScorePanel()
{
    delete ui;
}

void ScorePanel::setPlayerName(QByteArray left, QByteArray mid, QByteArray right)
{
    ui->leftName->setText(left);
    ui->myName->setText(mid);
    ui->rightName->setText(right);
}

void ScorePanel::setScore(int left, int right, int user)
{
    ui->labscore_1->setText(QString("%1").arg(user));
    ui->labscore_2->setText(QString("%1").arg(left));
    ui->labscore_3->setText(QString("%1").arg(right));
}

void ScorePanel::setFontSize(int point)
{
    QFont font = ui->labscore_1->font();
    font.setPointSize(point);
    for (auto label : list_) {
        label->setFont(font);
    }
}

void ScorePanel::setFontColor(Qt::GlobalColor color)
{
    QPalette palette = ui->labscore_1->palette();
    palette.setColor(QPalette::WindowText, color);
    for (auto label : list_) {
        label->setPalette(palette);
    }
}
