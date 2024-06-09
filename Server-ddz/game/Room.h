//
// Created by 38491 on 2024/4/15.
//

#ifndef SERVER_DDZ_ROOM_H
#define SERVER_DDZ_ROOM_H

#include <string>
#include <sw/redis++/redis++.h>

class Room {
public:
    Room() = default;
    ~Room();

    // 初始化 --连接Redis(redis++)
    bool initEnvironment();
    // 清空数据
    void clear();
    // 保存Rsa密钥对
    void saveRsaSecKey(std::string field, std::string value);
    // 读取rsa公/私钥
    std::string rsaSecKey(std::string field);
    // 加入房间
    std::string joinRoom(std::string userName);
    bool joinRoom(const std::string& userName, const std::string& roomName);
    // 随机生成房间名
    std::string getNewRoomName();
    // 获取某个房间内玩家的数量
    int getPlayerCount(std::string roomName);
    // 存储玩家的分数
    void updatePlayerScore(std::string roomName, std::string userName, int score);
    // 通过玩家名找房间名
    std::string whereAmI(std::string userName);
    // 查询玩家分数
    int playerScore(std::string roomName, std::string userName);
    // 从某个房间中删除用户
    void quitRoom(std::string roomName, std::string userName);
    // 获取抢地主的顺序
    std::string playersOrder(std::string roomName);
    // 搜索房间
    bool searchRoom(const std::string& roomName);

private:
    sw::redis::Redis *redis_;

    const std::string OnePlayer{"OnePlayer"};
    const std::string TwoPlayers{"TwoPlayers"};
    const std::string ThreePlayers{"ThreePlayers"};
    const std::string Invalid{"Invalid"};
};


#endif //SERVER_DDZ_ROOM_H
