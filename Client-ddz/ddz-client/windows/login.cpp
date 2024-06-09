#include "login.h"
#include "communication.h"
#include "datamanager.h"
#include "gamemode.h"
#include "ui_login.h"
#include <QCryptographicHash>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QThreadPool>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentIndex(0);

    connect(ui->homeBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->registerUser, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->netSettingBtn, &QPushButton::clicked, this, [=](){
        ui->stackedWidget->setCurrentIndex(2);
    });

    // 用户名
    QRegularExpression exp("^[a-zA-Z0-9_]{3,16}$");
    QRegularExpressionValidator *val1 = new QRegularExpressionValidator(exp, this);
    ui->userName->setValidator(val1);
    ui->userName_2->setValidator(val1);

    /*
     * 密码：
     * 1. 长度4-20
     * 2. 至少一个大写字母、小写字母、数字和特殊字符
    */
    exp.setPattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{4,20}$");
    QRegularExpressionValidator *val2 = new QRegularExpressionValidator(exp, this);
    ui->password->setValidator(val2);
    ui->password_2->setValidator(val2);

    // 手机号
    exp.setPattern("^1[34578]\\d{9}$");
    QRegularExpressionValidator *val3 = new QRegularExpressionValidator(exp, this);
    ui->phoneNum->setValidator(val3);

    // IP
    exp.setPattern("^((\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.){3}(\\d|[1-9]\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QRegularExpressionValidator *val4 = new QRegularExpressionValidator(exp, this);
    ui->IPaddress->setValidator(val4);

    // 端口
    exp.setPattern("^(0|([1-9]\\d{0,3})|([1-5]\\d{4})|(6[0-4]\\d{3})|(65[0-4]\\d{2})|(655[0-2]\\d)|(6553[0-5]))$");
    QRegularExpressionValidator *val5 = new QRegularExpressionValidator(exp, this);
    ui->port->setValidator(val5);

    connect(ui->login, &QPushButton::clicked, this, &Login::onLogin);
    connect(ui->register_2, &QPushButton::clicked, this, &Login::onRegister);
    connect(ui->confirm, &QPushButton::clicked, this, &Login::onNetSetting);

    // 设置线程池最大数
    QThreadPool::globalInstance()->setMaxThreadCount(8);

    // 加载数据
    loadUserInfo();
}


Login::~Login()
{
    delete ui;
}

bool Login::varifyData(QLineEdit *edit)
{
    if (edit->hasAcceptableInput() == false) {
        // 加红边框提示输入错误
        edit->setStyleSheet("border: 2px solid red;");
        return edit->hasAcceptableInput();
    }
    edit->setStyleSheet("none");
    return edit->hasAcceptableInput();
}

void Login::startConnect(Message *msg)
{
    if (!isConnect_) {
        Communication *task = new Communication(*msg);
        connect(task, &Communication::failed, this, [=](){
            QMessageBox::critical(this, "连接服务器", "连接服务器失败");
            isConnect_ = false;
        });
        connect(task, &Communication::loginOK, this, [=](){
            // 将当前用户名保存到单例对象
            DataManager::getInstance()->setUserName(ui->userName->text().toUtf8());
            // 保存用户名和密码
            saveUserInfo();
            // 选择游戏模式: 单机/联机
            GameMode *mode = new GameMode();
            mode->show();
            this->accept();
        });
        connect(task, &Communication::registerOK, this, [=](){
            ui->stackedWidget->setCurrentIndex(0);
        });
        connect(task, &Communication::failedMsg, this, [=](QByteArray msg){
            QMessageBox::critical(this, "ERROR", msg);
        });
        isConnect_ = true;
        QThreadPool::globalInstance()->start(task);
        DataManager::getInstance()->setCommunication(task);
    }
    else {
        // 和服务器通信
        DataManager::getInstance()->communication()->sendMessage(msg, true);
    }
}

void Login::saveUserInfo()
{
    if (ui->savePwd->isChecked()) {
        QJsonObject obj;
        obj.insert("user", ui->userName->text());
        obj.insert("passwd", ui->password->text());
        QJsonDocument doc(obj);
        QByteArray json = doc.toJson();
        // aes加密
        AesCrypto aes(AesCrypto::Algorithm::aes_128_cbc, KEY_.left(16));
        json = aes.encrypt(json);
        // 写文件
        QFile file("passwd.bin");
        file.open(QFile::WriteOnly);            // WriteOnly模式下，open会自动创建不存在的文件
        file.write(json);
        file.close();
    }
    else {
        QFile file("passwd.bin");
        file.remove();
    }
}

void Login::loadUserInfo()
{
    QFile file("passwd.bin");
    bool flag = file.open(QFile::ReadOnly);     // ReadOnly模式下，open不会自动创建不存在的文件
    if (!flag) {
        ui->savePwd->setChecked(false);
        return;
    }

    // 读json数据 & aes解密
    ui->savePwd->setChecked(true);              // 勾选记住密码
    QByteArray json = file.readAll();
    AesCrypto aes(AesCrypto::aes_128_cbc, KEY_.left(16));
    json = aes.decrypt(json);
    file.close();

    // 加载数据
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();
    ui->userName->setText(obj.value("user").toString());
    ui->password->setText(obj.value("passwd").toString());
}

void Login::onLogin()
{
    bool f1 = varifyData(ui->userName);
    bool f2 = varifyData(ui->password);
    if (f1 && f2) {         // 登录成功
        Message msg;
        msg.userName = ui->userName->text().toUtf8();
        msg.requestCode = RequestCode::UserLogin;
        QByteArray password = ui->password->text().toUtf8();
        password = QCryptographicHash::hash(password, QCryptographicHash::Sha224).toHex();
        msg.data1 = password;
        // 连接服务器
        startConnect(&msg);
    }
}

void Login::onRegister()
{
    bool f1 = varifyData(ui->userName_2);
    bool f2 = varifyData(ui->password_2);
    bool f3 = varifyData(ui->phoneNum);
    if (f1 && f2 && f3) {   // 注册成功
        Message msg;
        msg.userName = ui->userName_2->text().toUtf8();
        msg.requestCode = RequestCode::Register;
        msg.data2 = ui->phoneNum->text().toUtf8();
        QByteArray password = ui->password_2->text().toUtf8();
        password = QCryptographicHash::hash(password, QCryptographicHash::Sha224).toHex();
        msg.data1 = password;
        // 连接服务器
        startConnect(&msg);
    }
}

void Login::onNetSetting()
{
    bool f1 = varifyData(ui->IPaddress);
    bool f2 = varifyData(ui->port);
    if (f1 && f2) {         // 网络设置成功
        DataManager *instance = DataManager::getInstance();     // 获得全局唯一实例
        instance->setIp(ui->IPaddress->text().toUtf8());
        instance->setPort(ui->port->text().toUtf8());
    }
}
