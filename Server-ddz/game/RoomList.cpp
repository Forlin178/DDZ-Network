//
// Created by 38491 on 2024/4/18.
//

#include "RoomList.h"
#include "Log.h"

RoomList *RoomList::getInstance() {
    static RoomList obj;
    return &obj;
}

void RoomList::addUser(const std::string& roomName, std::string userName, callback sendMessage) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (roomMap_.count(roomName) >= 1) {
        Debug("RoomMap: insert new member <%s> to Room: %s", userName.data(), roomName.data());
        auto &value = roomMap_[roomName];
        value.insert(std::make_pair(userName, sendMessage));
    } else {
        Debug("RoomMap: first member <%s> of new Room: %s", userName.data(), roomName.data());
        UserMap value = {{userName, sendMessage}};
        roomMap_.insert(std::make_pair(roomName, value));
    }
}

UserMap RoomList::getPlayers(const std::string& roomName) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (roomMap_.count(roomName) >= 1)
        return roomMap_[roomName];
    return {};
}

UserMap RoomList::getPartners(const std::string& roomName, const std::string& userName) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (roomMap_.count(roomName) >= 1) {
        const auto &roomMember = roomMap_[roomName];
        UserMap partners;
        for (const auto& iter: roomMember) {
            if (iter.first != userName)
                partners.insert(iter);
        }
        return partners;
    }
    return {};
}

void RoomList::removePlayer(const std::string& roomName, const std::string& userName) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (roomMap_.count(roomName) > 0) {
        auto &userMap = roomMap_[roomName];     // 别忘了引用，不然删除的就是副本里的元素
        if (userMap.count(userName) > 0 && userMap.size() > 1) {
            userMap.erase(userName);
        } else if (userMap.count(userName) > 0 && userMap.size() == 1) {
            roomMap_.erase(roomName);
        }
    }
}

void RoomList::removeRoom(const std::string &roomName) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (roomMap_.count(roomName) > 0) {
        roomMap_.erase(roomName);
    }
}
