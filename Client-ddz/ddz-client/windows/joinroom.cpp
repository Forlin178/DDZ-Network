#include "joinroom.h"
#include "codec.h"
#include "datamanager.h"
#include "ui_joinroom.h"

JoinRoom::JoinRoom(DialogType type, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::JoinRoom)
{
    ui->setupUi(this);

    const QString titles[] = {
        "创建房间的名字: ",
        "搜索房间的名字: "
    };
    int index = type == DialogType::Manual ? 0 : 1;
    ui->describe->setText(titles[index]);
    ui->stackedWidget->setCurrentIndex(index);

    setFixedSize(300, 180);

    Communication *comm = DataManager::getInstance()->communication();
    connect(comm, &Communication::roomExist, this, [=](bool flag){
        ui->join_btn->setEnabled(flag);
    });

    connect(ui->create_btn, &QPushButton::clicked, this, &JoinRoom::joinRoom);
    connect(ui->search_btn, &QPushButton::clicked, this, &JoinRoom::searchRoom);
    connect(ui->join_btn, &QPushButton::clicked, this, &JoinRoom::joinRoom);
}

JoinRoom::~JoinRoom()
{
    delete ui;
}

void JoinRoom::searchRoom()
{
    encodeMsg(RequestCode::SearchRoom);
}

void JoinRoom::joinRoom()
{
    encodeMsg(RequestCode::ManualRoom);
    DataManager::getInstance()->setRoomMode(DataManager::Manual);
    accept();
}

void JoinRoom::encodeMsg(RequestCode code)
{
    Message msg;
    msg.requestCode = code;
    msg.userName = DataManager::getInstance()->userName();
    msg.roomName = ui->lineEdit->text().toUtf8();
    DataManager::getInstance()->communication()->sendMessage(&msg, true);
}
