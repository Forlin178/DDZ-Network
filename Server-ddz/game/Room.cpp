//
// Created by 38491 on 2024/4/15.
//

#include "Room.h"
#include "JsonParse.h"
#include "Log.h"
#include <random>

Room::~Room() {
    delete redis_;
}

bool Room::initEnvironment() {
    // tcp://127.0.0.1:6379
    JsonParse json;
    auto info = json.getDBInfo(JsonParse::Redis);
    std::string uri = "tcp://" + info->ip + ":" + std::to_string(info->port);
    redis_ = new sw::redis::Redis(uri);

    // 测试连接
    if (redis_->ping() == "PONG") {
        Debug("Redis: 服务器连接成功");
        return true;
    }
    return false;
}

void Room::clear() {
    redis_->flushdb();
}

void Room::saveRsaSecKey(std::string field, std::string value) {
    redis_->hset("RSA", field, value);
}

std::string Room::rsaSecKey(std::string field) {
    auto value = redis_->hget("RSA", field);
    if (value.has_value())
        return value.value();
    return {};
}

std::string Room::joinRoom(std::string userName) {
    // 搜索2人间 -> 1人间 -> 创建新房间
    std::optional<std::string> room;
    if (redis_->scard(TwoPlayers) > 0) {            // 获得2人间房名
        room = redis_->srandmember(TwoPlayers);
    } else if (redis_->scard(OnePlayer) > 0) {      // 获得1人间房名
        room = redis_->srandmember(OnePlayer);
    } else {                                        // 创建房间
        room = getNewRoomName();
    }
    // 加入到房间中
    joinRoom(userName, room.value());
    Debug("%s", room.value().data());
    return room.value();
}

bool Room::joinRoom(const std::string &userName, const std::string &roomName) {
    if (redis_->zcard(roomName) >= 3) {     // 房间满员了
        return false;
    }
    if (!redis_->exists(roomName)) {        // 创建房间
        redis_->sadd(OnePlayer, roomName);
    } else if (redis_->sismember(OnePlayer, roomName)) {
        redis_->smove(OnePlayer, TwoPlayers, roomName);
    } else if (redis_->sismember(TwoPlayers, roomName)) {
        redis_->smove(TwoPlayers, ThreePlayers, roomName);
    } else {
        assert(false);
    }

    redis_->hset("Players", userName, roomName);    // 更新/添加 user与房间名的映射
    redis_->zadd(roomName, userName, 0);            // 更新/添加 user到新房间set
    return true;
}

std::string Room::getNewRoomName() {
    std::random_device rand;            // 创建随机设备对象
    std::mt19937 gen(rand());           // 创建随机数生成对象
    std::uniform_int_distribution<int> dis(100000, 999999);     // 创建随机数分布对象：均匀分布
    int randomNum = dis(gen);
    return std::to_string(randomNum);
}

int Room::getPlayerCount(std::string roomName) {
    return redis_->zcard(roomName);
}

void Room::updatePlayerScore(std::string roomName, std::string userName, int score) {
    redis_->zadd(roomName, userName, score);
}

std::string Room::whereAmI(std::string userName) {
    auto roomName = redis_->hget("Players", userName);
    if (roomName.has_value())
        return roomName.value();
    return {};
}

int Room::playerScore(std::string roomName, std::string userName) {
    auto score = redis_->zscore(roomName, userName);
    if (score.has_value())
        return score.value();
    return 0;
}

void Room::quitRoom(std::string roomName, std::string userName) {
    if (redis_->sismember(ThreePlayers, roomName)) {
        redis_->smove(ThreePlayers, Invalid, roomName);
    }
    // 从房间中删除玩家
    redis_->zrem(roomName, userName);
    int count = redis_->zcard(roomName);
    if (count == 0) {
        redis_->del(roomName);              // 将名为roomName的SortedSet删除
        redis_->srem(Invalid, roomName);    // 将roomName从Invalid集合中删除
    }
}

std::string Room::playersOrder(std::string roomName) {
    std::string data;
    int count = 0;
    std::vector<std::pair<std::string, double>> vec;
    redis_->zrevrange(roomName, 0, -1, std::back_inserter(vec));
    for (const auto &it: vec) {
        data += it.first + "-" +
                std::to_string(++count) + "-" +
                std::to_string((int) it.second) + "#";
    }
    return data;
}

bool Room::searchRoom(const std::string& roomName) {
    bool flag1 = redis_->sismember(TwoPlayers, roomName);
    bool flag2 = redis_->sismember(OnePlayer, roomName);
    return flag1 || flag2;
}
