//
// Created by 38491 on 2024/4/10.
//

#ifndef SERVER_DDZ_COMMUNICATION_H
#define SERVER_DDZ_COMMUNICATION_H

#include "Codec.h"
#include "Buffer.h"
#include "AesCrypto.h"
#include "MysqlConn.h"
#include "Room.h"
#include "RoomList.h"

class Communication {
    using sendCallback = std::function<void(const std::string &)>;
    using deleteCallback = std::function<void(void)>;
public:
    Communication();

    ~Communication();

    void setCallback(sendCallback sendFunc, deleteCallback deleteFunc);

    void parseRequest(Buffer *buf);

    void handleAesFenFa(Message *msg, Message &responseMsg);

    void handleRegister(Message *msg, Message &responseMsg);

    void handleLogin(Message *msg, Message &responseMsg);

    void handleAddRoom(Message *msg, Message &responseMsg);

    void handleLeaveRoom(Message *msg, Message &responseMsg);

    void handleGoodbye(Message *msg);

    // 转发消息
    void notifyOtherPlayers(const std::string& data, const std::string& roomName, const std::string& userName);
    // 准备开始游戏
    void readyForPlay(const std::string& roomName, const std::string& data);
    // 发牌
    void dealCards(const UserMap& players);
    // 洗牌
    void initCards();
    // 随机取一张牌
    std::pair<int, int> takeOneCard();
    // 重新开始游戏
    void restartGame(Message *msg);
    // 开始游戏
    void startGame(const UserMap& players, const std::string& roomName);

private:
    std::string aesKey_;
    AesCrypto *aes_{nullptr};
    MysqlConn *mysql_{nullptr};
    Room *redis_{nullptr};
    std::multimap<int, int> cards_;     // 花色-点数
    sendCallback sendFunc_;
    deleteCallback deleteFunc_;
};


#endif //SERVER_DDZ_COMMUNICATION_H
