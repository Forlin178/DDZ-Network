#pragma once

#include "Channel.h"
#include "Communication.h"
#include <string>
#include "EventLoop.h"
//#define MSG_SEND_DATA

class TcpConnection {
public:
    //初始化
    TcpConnection(int fd, EventLoop *evLoop);

    ~TcpConnection();

    // 发送数据
    void addWriteTask(const std::string& data);
    // 释放资源
    void addDeleteTask();

    // 准备密钥
    void prepareSecretKey();

    static int processRead(void *arg);

    static int processWrite(void *arg);

    static int destroy(void *arg);

private:
    std::string name_;
    EventLoop *evLoop_;
    Channel *channel_;
    Buffer *readBuf_;
    Buffer *writeBuf_;
    Communication *reply_{nullptr};
};

