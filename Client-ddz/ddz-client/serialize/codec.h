#ifndef CODEC_H
#define CODEC_H

#include <QByteArray>
#include <QSharedPointer>
#include "information.pb.h"

struct Message
{
    // 序列化   -> 基于该结构体进行
    // 反序列化 -> 解析出的数据存储到这里
    QByteArray userName;
    QByteArray roomName;
    QByteArray data1;
    QByteArray data2;
    QByteArray data3;
    RequestCode requestCode;
    ResponseCode responseCode;
};

class Codec
{
public:
    Codec(Message *msg);        // 序列化
    Codec(QByteArray msg);      // 反序列化

    QByteArray encodeMsg();                    // 序列化
    QSharedPointer<Message> decodeMsg();       // 反序列化

    void reload(Message *msg);
    void reload(QByteArray msg);

private:
    QByteArray msg_;
    Information obj_;           // 用于数据的序列化和反序列化
};

#endif // CODEC_H
