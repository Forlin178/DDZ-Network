#include "gamemode.h"
#include "gamepanel.h"
#include "datamanager.h"
#include "codec.h"
#include "ui_gamemode.h"
#include "joinroom.h"
#include <QCloseEvent>

GameMode::GameMode(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GameMode)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);
    ui->information->hide();

    auto comm = DataManager::getInstance()->communication();

    // 处理JoinRoom信息
    connect(comm, &Communication::playerCount, this, [=](int number){
        QString tip = QString("当前<%1>房间玩家人数为%2，正在等待其他玩家...").
                      arg(DataManager::getInstance()->roomName().data()).
                      arg(number);
        ui->information->setText(tip);
        ui->information->setVisible(true);
    });

    connect(comm, &Communication::someoneLeave, this, [=](int count){
        QString tip = QString("当前<%1>房间玩家人数为%2，正在等待其他玩家...").
                      arg(DataManager::getInstance()->roomName().data()).
                      arg(count);
        ui->information->setText(tip);
    });

    // 处理StartGame信息
    connect(comm, &Communication::startGame, this, [=](QByteArray msg){
        this->hide();
        GamePanel *panel = new GamePanel();
        connect(panel, &GamePanel::panelClose, this, &GamePanel::show);
        panel->show();
        panel->initGamePanel(msg);
        // 断开信号槽 --下次startGame是再开一局，不由该窗口处理
        // 在GamePanel构造时重新构建连接
        disconnect(comm, &Communication::startGame, this, nullptr);
    });

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [=](int index){
        if (index == 0)
            ui->information->hide();
    });

    // 单机模式
    connect(ui->singlemode_btn, &QPushButton::clicked, this, [=](){
        GamePanel *panel = new GamePanel();
        connect(panel, &GamePanel::panelClose, this, &GameMode::show);
        panel->show();
        this->hide();
        DataManager::getInstance()->setMode(DataManager::Single);
    });

    // 联机模式
    connect(ui->netmode_btn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
        DataManager::getInstance()->setMode(DataManager::Network);
    });

    // 自动加入某个房间
    connect(ui->auto_btn, &QPushButton::clicked, this, [=](){
        Message msg;
        msg.userName = DataManager::getInstance()->userName();
        msg.requestCode = RequestCode::AutoRoom;
        DataManager::getInstance()->communication()->sendMessage(&msg, true);
        DataManager::getInstance()->setRoomMode(DataManager::Auto);
    });
    // 创建房间
    connect(ui->manual_btn, &QPushButton::clicked, this, [=](){
        JoinRoom room(DialogType::Manual);
        room.setWindowTitle("创建房间");
        room.exec();
    });
    // 搜索房间
    connect(ui->search_btn, &QPushButton::clicked, this, [=](){
        JoinRoom room(DialogType::Search);
        room.setWindowTitle("搜索房间");
        room.exec();
    });
}

GameMode::~GameMode()
{
    delete ui;
}

void GameMode::closeEvent(QCloseEvent *event)
{
    if (ui->stackedWidget->currentIndex() == 1) {
        // 在选择加入房间的界面退出后会切换回选择联网/单机界面，不会真的退出登录
        ui->stackedWidget->setCurrentIndex(0);
        event->ignore();
    }
    else {
        // 在选择联网/单机界面退出时，用户退出登录
        event->accept();
        Message msg;
        msg.requestCode = RequestCode::Goodbye;
        msg.userName = DataManager::getInstance()->userName();
        msg.roomName = DataManager::getInstance()->roomName();
        DataManager::getInstance()->communication()->sendMessage(&msg, true);
        DataManager::getInstance()->communication()->stopLoop();

    }
}
