//
// Created by 38491 on 2024/4/7.
//

#include "Codec.h"
#include <iostream>
#include <utility>

Codec::Codec(Message *msg) {
    reload(msg);
}

Codec::Codec(std::string msg) {
    reload(std::move(msg));
}

std::string Codec::encodeMsg() {
    obj_.SerializeToString(&msg_);       // 将 obj 的内容序列化并存储在 string 中
    return msg_;
}

std::shared_ptr<Message> Codec::decodeMsg() {
    obj_.ParseFromString(msg_);          // 解析 string 内容并存储到 obj 内
    auto *msg = new Message;
    msg->userName = obj_.username();
    msg->roomName = obj_.roomname();
    msg->data1 = obj_.data1();
    msg->data2 = obj_.data2();
    msg->data3 = obj_.data3();
    msg->requestCode = obj_.requestcode();
    msg->responseCode = obj_.responsecode();

    // std::shared_ptr原来也能自定义删除啊
    std::shared_ptr<Message> ptr(msg, [this](Message *pt) {
        delete pt;
        std::cout << "delete message occur...";
    });
    return ptr;
}

void Codec::reload(Message *msg) {
    obj_.set_username(msg->userName);
    obj_.set_roomname(msg->roomName);
    obj_.set_data1(msg->data1);
    obj_.set_data2(msg->data2);
    obj_.set_data3(msg->data3);
    obj_.set_requestcode(msg->requestCode);
    obj_.set_responsecode(msg->responseCode);
}

void Codec::reload(std::string msg) {
    msg_ = std::move(msg);
}
