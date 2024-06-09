//
// Created by 38491 on 2024/4/18.
//

#ifndef SERVER_DDZ_ROOMLIST_H
#define SERVER_DDZ_ROOMLIST_H

#include <map>
#include <string>
#include <functional>
#include <mutex>

using callback = std::function<void(const std::string&)>;
using UserMap = std::map<std::string, callback>;

class RoomList {    // 单例模式-懒汉
public:
    RoomList(const RoomList&) = delete;
    RoomList& operator=(const RoomList&) = delete;
    static RoomList* getInstance();

    // 添加用户
    void addUser(const std::string& roomName, std::string userName, callback sendMessage);
    // 通过房间名取出玩家信息
    UserMap getPlayers(const std::string& roomName);
    // 得到当前房间内除指定玩家以外的玩家
    UserMap getPartners(const std::string& roomName, const std::string& userName);
    // 删除指定房间内的玩家
    void removePlayer(const std::string& roomName, const std::string& userName);
    // 清空房间内的玩家
    void removeRoom(const std::string& roomName);

private:
    RoomList() = default;
    std::map<std::string, UserMap> roomMap_;    // 共享资源
    std::mutex mutex_;                          // 保护共享资源的互斥锁
};


#endif //SERVER_DDZ_ROOMLIST_H
