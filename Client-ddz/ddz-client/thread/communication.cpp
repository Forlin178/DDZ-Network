#include "communication.h"
#include "card.h"
#include "cards.h"
#include "datamanager.h"
#include "rsacrypto.h"
#include "taskqueue.h"
#include <QThread>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QCryptographicHash>

Communication::Communication(Message msg, QObject *parent)
    : QObject{parent}
{
    setAutoDelete(true);
    msg_ = msg;
}

Communication::~Communication()
{
    if (aes_ != nullptr)
        delete aes_;
}

void Communication::sendMessage(Message *msg, bool encrypt)
{
    Codec codec(msg);
    QByteArray data = codec.encodeMsg();    // 序列化
    qDebug() << data;
    // 对称加密，只有分发对称密钥时不需要经过对称加密
    if (encrypt) {
        data = aes_->encrypt(data);
    }
    socket_->sendMsg(data);                 // 发送序列化数据
}

void Communication::parseRecvMessage()
{
    QByteArray recvData = socket_->recvMsg(2);
    if (recvData.isEmpty()) {
        return;
    }
    
    Codec codec(recvData);
    auto msg = codec.decodeMsg();   // 反序列化
    switch (msg->responseCode) {
    case ResponseCode::LoginOK:
        emit loginOK();
        break;
    case ResponseCode::RegisterOK:
        emit registerOK();
        break;
    case ResponseCode::RsaFenFa:
        handleRsaFenFa(msg.get());
        break;
    case ResponseCode::AesVerifyOK: {
        aes_ = new AesCrypto(AesCrypto::aes_256_cbc, aesKey_);
        sendMessage(&msg_, true);          // 完成了非对称/对称密钥的分发，才能开始发送登录/注册request
        break;
    }
    case ResponseCode::JoinRoomOK:{
        DataManager::getInstance()->setRoomName(msg->roomName);
        emit playerCount(msg->data1.toInt());
        break;
    }
    case ResponseCode::DealCards:
        parseCards(msg->data1, msg->data2);
        break;
    case ResponseCode::StartGame:
        emit startGame(msg->data1);
        break;
    case ResponseCode::OtherGrabLoad: {
        Task t;
        t.bet = msg->data1.toInt();
        TaskQueue::getInstance()->add(t);
        break;
    }
    case ResponseCode::OtherPlayHand: {
        Task t;
        int num = msg->data1.toInt();
        QDataStream stream(msg->data2);
        for (int i = 0; i < num; i++) {
            Card c;
            stream >> c;
            t.cs << c;
        }
        TaskQueue::getInstance()->add(t);
        break;
    }
    case ResponseCode::SearchRoomOK: {
        bool flag = msg->data1 == "true";
        emit roomExist(flag);
        break;
    }
    case ResponseCode::OtherLeaveRoom: {
        int count = msg->data1.toInt();
        emit someoneLeave(count);
        break;
    }
    case ResponseCode::Failed:
        emit failedMsg(msg->data1);
        break;
    default:
        break;
    }
}

void Communication::handleRsaFenFa(Message *msg)
{
    // 校验签名
    RsaCrypto rsa;
    rsa.parseStringToKey(msg->data1, RsaCrypto::PublicKey);
    bool flag = rsa.varify(msg->data2, msg->data1);
    if (!flag) {
        QMessageBox::information(nullptr, "校验失败", "签名校验失败");
    }

    // 生成对称加密的密钥并分发，用非对称公钥加密
    aesKey_ = generateAesKey(KeyLen::L32);
    Message aesMsg;
    aesMsg.requestCode = RequestCode::AesFenFa;
    aesMsg.data1 = rsa.pubKeyEncrypt(aesKey_);
    QCryptographicHash hash(QCryptographicHash::Sha224);
    hash.addData(aesKey_);
    aesMsg.data2 = hash.result().toHex();

    // 发送序列化数据
    sendMessage(&aesMsg, false);
}

QByteArray Communication::generateAesKey(KeyLen keyLen)
{
    QByteArray time = QDateTime::currentDateTime().toString("yyyy.MM.dd-hh:mm:ss.zzz").toUtf8();
    QCryptographicHash hash(QCryptographicHash::Sha384);    // 48字节,满足aes密钥3个档位(16/24/32)
    hash.addData(time);
    time = hash.result();           // 根据当前时间生成哈希值
    time = time.left(keyLen);       // 取高位keyLen作为对称加密秘钥
    return time;
}

void Communication::parseCards(QByteArray data1, QByteArray data2)
{
    auto func = std::bind([=](QByteArray msg){
        auto lst = msg.left(msg.size() - 1).split('#');
        Cards cards;
        for (const auto& iter : lst) {
            auto sub = iter.split('-');
            Card card(static_cast<Card::CardPoint>(sub[1].toInt()),
                      static_cast<Card::CardSuit>(sub[0].toInt()));
            cards << card;
        }
        return cards;
    }, std::placeholders::_1);

    // 将发牌信息存储到全局(单例)内
    DataManager::getInstance()->setCards(func(data1));
    DataManager::getInstance()->setLast(func(data2));
}

void Communication::run()
{
    socket_ = new TcpSocket();
    bool flag = socket_->connectToHost(DataManager::getInstance()->ip(),
                                       DataManager::getInstance()->port().toInt());
    if (!flag) {
        emit failed();     // 通知主线程连接失败
        socket_->deleteLater();
        return;
    }
    qDebug() << "======= 通信的子线程ID:" << QThread::currentThreadId() << "connect state:" << flag << "========";

    // 连接成功，处理服务器发送的消息
    while (!stop_) {
        // 得到来自服务器端的非对称加密公钥
        // 返回用公钥加密对称加密的密钥给服务端
        // 服务器回复确认后再发送登录或注册信息请求
        parseRecvMessage();
    }
    socket_->disconnect();
    socket_->deleteLater();
}
