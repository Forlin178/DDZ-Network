#include "codec.h"
#include <QDebug>

Codec::Codec(Message *msg)
{
    reload(msg);
}

Codec::Codec(QByteArray msg)
{
    reload(msg);
}

QByteArray Codec::encodeMsg()
{
    std::string str;
    obj_.SerializeToString(&str);       // 将 obj 的内容序列化并存储在 string 中
    return QByteArray::fromStdString(str);
}

QSharedPointer<Message> Codec::decodeMsg()
{
    std::string str = msg_.toStdString();
    obj_.ParseFromString(str);          // 解析 string 内容并存储到 obj 内
    Message *msg = new Message;
    msg->userName = QByteArray::fromStdString(obj_.username());
    msg->roomName = QByteArray::fromStdString(obj_.roomname());
    msg->data1 = QByteArray::fromStdString(obj_.data1());
    msg->data2 = QByteArray::fromStdString(obj_.data2());
    msg->data3 = QByteArray::fromStdString(obj_.data3());
    msg->requestCode = obj_.requestcode();
    msg->responseCode = obj_.responsecode();

    QSharedPointer<Message> ptr(msg, [this](Message *pt){
        delete pt;
        qDebug() << "delete message occur...";
    });
    return ptr;
}

void Codec::reload(Message *msg)
{
    obj_.set_username(msg->userName.toStdString());
    obj_.set_roomname(msg->roomName.toStdString());
    obj_.set_data1(msg->data1.toStdString());
    obj_.set_data2(msg->data2.toStdString());
    obj_.set_data3(msg->data3.toStdString());
    obj_.set_requestcode(msg->requestCode);
    obj_.set_responsecode(msg->responseCode);
}

void Codec::reload(QByteArray msg)
{
    msg_ = msg;
}
