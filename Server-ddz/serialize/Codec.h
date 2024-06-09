//
// Created by 38491 on 2024/4/7.
//

#ifndef SERVER_DDZ_CODEC_H
#define SERVER_DDZ_CODEC_H

#include <string>
#include <memory>
#include "information.pb.h"

struct Message {
    // 序列化   -> 基于该结构体进行
    // 反序列化 -> 解析出的数据存储到这里
    std::string userName;
    std::string roomName;
    std::string data1;
    std::string data2;
    std::string data3;
    RequestCode requestCode;
    ResponseCode responseCode;
};

class Codec {
public:
    Codec(Message *msg);         // 序列化
    Codec(std::string msg);      // 反序列化

    std::string encodeMsg();                    // 序列化
    std::shared_ptr<Message> decodeMsg();       // 反序列化

    void reload(Message *msg);

    void reload(std::string msg);

private:
    std::string msg_;
    Information obj_;           // 用于数据的序列化和反序列化
};

#endif //SERVER_DDZ_CODEC_H
