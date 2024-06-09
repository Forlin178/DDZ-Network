#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QObject>
#include <QRunnable>
#include "codec.h"
#include "tcpsocket.h"
#include "aescrypto.h"

class Communication : public QObject, public QRunnable
{
    Q_OBJECT
public:
    enum KeyLen {L16 = 16, L24 = 24, L32 = 32}; // 对称加密的密钥长度
    explicit Communication(Message msg, QObject *parent = nullptr);

    ~Communication();

    inline void stopLoop() { stop_ = true; }

    void sendMessage(Message *msg, bool encrypt);     // 发送序列化数据到服务器

    void parseRecvMessage();            // 解析来自服务器的数据

    void handleRsaFenFa(Message *msg);  // 处理来自服务端分发的公钥

    QByteArray generateAesKey(KeyLen keyLen);   // 生成对称加密的密钥

    void parseCards(QByteArray data1, QByteArray data2);      // 解析卡牌信息 --DealCards

signals:
    void failed();
    void loginOK();
    void registerOK();
    void failedMsg(QByteArray msg);
    void playerCount(int number);
    void startGame(QByteArray msg);
    void roomExist(bool flag);
    void someoneLeave(int count);

    // QRunnable interface
public:
    virtual void run() override;

private:
    TcpSocket *socket_{ nullptr };
    Message msg_;
    bool stop_{ false };
    QByteArray aesKey_;
    AesCrypto *aes_{nullptr};
};


#endif // COMMUNICATION_H
