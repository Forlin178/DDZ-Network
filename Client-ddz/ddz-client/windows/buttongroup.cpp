#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::initButton()
{
    // btn-开始游戏
    ui->btnStart->setImage(":/image/images/start-1.png", ":/image/images/start-3.png", ":/image/images/start-2.png");
    // btn-出牌
    ui->btnPlayCard->setImage(":/image/images/chupai_btn-1.png", ":/image/images/chupai_btn-3.png", ":/image/images/chupai_btn-2.png");
    ui->btnPlayCard_2->setImage(":/image/images/chupai_btn-1.png", ":/image/images/chupai_btn-3.png", ":/image/images/chupai_btn-2.png");
    // btn-要不起
    ui->btnPass->setImage(":/image/images/pass_btn-1.png", ":/image/images/pass_btn-3.png", ":/image/images/pass_btn-2.png");
    // btn-抢地主-不抢
    ui->btnGiveUP->setImage(":/image/images/buqiang-1.png", ":/image/images/buqiang-3.png", ":/image/images/buqiang-2.png");
    // btn-抢地主-1/2/3分
    ui->btnOneBet->setImage(":/image/images/1fen-1.png", ":/image/images/1fen-3.png", ":/image/images/1fen-2.png");
    ui->btnTwoBet->setImage(":/image/images/2fen-1.png", ":/image/images/2fen-3.png", ":/image/images/2fen-2.png");
    ui->btnThreeBet->setImage(":/image/images/3fen-1.png", ":/image/images/3fen-3.png", ":/image/images/3fen-2.png");

    // 设置大小
    QVector<MyButton*> vec;
    vec << ui->btnStart << ui->btnPlayCard << ui->btnPlayCard_2 << ui->btnPass << ui->btnGiveUP
        << ui->btnOneBet << ui->btnTwoBet << ui->btnThreeBet;
    for (const auto& iter : vec) {
        iter->setFixedSize(90, 45);
    }

    // 各个按钮点击之后会发射信号，不在这处理
    connect(ui->btnStart, &MyButton::clicked, this, [=](){
        emit startGame();
    });
    connect(ui->btnPlayCard, &MyButton::clicked, this, [=](){
        emit playHand();
    });
    connect(ui->btnPlayCard_2, &MyButton::clicked, this, [=](){
        emit playHand();
    });
    connect(ui->btnPass, &MyButton::clicked, this, [=](){
        emit pass();
    });
    connect(ui->btnGiveUP, &MyButton::clicked, this, [=](){
        emit betPoint(0);
    });
    connect(ui->btnOneBet, &MyButton::clicked, this, [=](){
        emit betPoint(1);
    });
    connect(ui->btnTwoBet, &MyButton::clicked, this, [=](){
        emit betPoint(2);
    });
    connect(ui->btnThreeBet, &MyButton::clicked, this, [=](){
        emit betPoint(3);
    });
}

void ButtonGroup::selectPanel(Panel type, int bet)
{
    ui->stackedWidget->setCurrentIndex(type);
    if (type == CallLord) {
        if (bet == 0) {
            ui->btnOneBet->setVisible(true);
            ui->btnTwoBet->setVisible(true);
            ui->btnThreeBet->setVisible(true);
        }
        else if (bet == 1) {
            ui->btnOneBet->setVisible(false);
            ui->btnTwoBet->setVisible(true);
            ui->btnThreeBet->setVisible(true);
        }
        else if (bet == 2) {
            ui->btnOneBet->setVisible(false);
            ui->btnTwoBet->setVisible(false);
            ui->btnThreeBet->setVisible(true);
        }
    }
}
