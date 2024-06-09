#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include "codec.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    bool varifyData(QLineEdit *edit);              // 验证数据
    void startConnect(Message *msg);               // 连接服务器

    void saveUserInfo();            // 保存用户数据
    void loadUserInfo();            // 加载用户数据(记住密码功能)

private slots:
    void onLogin();
    void onRegister();
    void onNetSetting();

private:
    Ui::Login *ui;
    bool isConnect_{ false };
    const QByteArray KEY_{"forlin38491815517675664423"};
};

#endif // LOGIN_H
