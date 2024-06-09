//
// Created by 38491 on 2024/4/10.
//

#include <netinet/in.h>
#include "Communication.h"
#include "Log.h"
#include "RsaCrypto.h"
#include "Hash.h"
#include "JsonParse.h"

void Communication::parseRequest(Buffer *buf) {
    // 读取序列化的数据: 数据头 + 数据块
    std::string head = buf->readData(sizeof(int));          // 取数据头
    int length = *(int *) head.data();                       // 大端数据
    length = ntohl(length);                                 // 小段数据
    std::string data = buf->readData(length);               // 取数据块

    if (aes_ != nullptr) {                                  // 对称解密
        data = aes_->decrypt(data);
    }

    // 反序列化
    Codec codec(data);
    auto msg = codec.decodeMsg();
    Message responseMsg;
    sendCallback myFunc_ = sendFunc_;
    switch (msg->requestCode) {
        case RequestCode::UserLogin: {
            handleLogin(msg.get(), responseMsg);
            break;
        }
        case RequestCode::Register: {
            handleRegister(msg.get(), responseMsg);
            break;
        }
        case RequestCode::AesFenFa: {
            handleAesFenFa(msg.get(), responseMsg);
            break;
        }
        case RequestCode::AutoRoom:
        case RequestCode::ManualRoom:
            handleAddRoom(msg.get(), responseMsg);
            myFunc_ = std::bind(&Communication::readyForPlay, this,
                                responseMsg.roomName, std::placeholders::_1);
            break;
        case RequestCode::GrabLoad: {     // 转发抢地主给房间里的其他玩家
            responseMsg.data1 = msg->data1;
            responseMsg.responseCode = ResponseCode::OtherGrabLoad;
            myFunc_ = std::bind(&Communication::notifyOtherPlayers, this,
                                std::placeholders::_1, msg->roomName, msg->userName);
            break;
        }
        case RequestCode::PlayAHand: {     // 转发抢地主给房间里的其他玩家
            responseMsg.data1 = msg->data1;
            responseMsg.data2 = msg->data2;
            responseMsg.responseCode = ResponseCode::OtherPlayHand;
            myFunc_ = std::bind(&Communication::notifyOtherPlayers, this,
                                std::placeholders::_1, msg->roomName, msg->userName);
            break;
        }
        case RequestCode::GameOver: {      // 更新数据库
            // redis
            int score = std::stoi(msg->data1);
            redis_->updatePlayerScore(msg->roomName, msg->userName, score);
            // mysql
            char sql[128];
            sprintf(sql, "UPDATE information set score = %d WHERE name = '%s'",
                    score, msg->userName.data());
            mysql_->update(sql);
            myFunc_ = nullptr;      // 更新数据库后不返回消息
            break;
        }
        case RequestCode::Continue:
            restartGame(msg.get());
            myFunc_ = nullptr;
            break;
        case RequestCode::SearchRoom: {
            bool flag = redis_->searchRoom(msg->roomName);
            responseMsg.responseCode = ResponseCode::SearchRoomOK;
            responseMsg.data1 = flag == true ? "true" : "false";
            break;
        }
        case RequestCode::LeaveRoom: {
            handleLeaveRoom(msg.get(), responseMsg);
            myFunc_ = nullptr;
            break;
        }
        case RequestCode::Goodbye:
            handleGoodbye(msg.get());
            myFunc_ = nullptr;
            break;
        default:
            break;
    }
    if (myFunc_) {
        Codec resCodec(&responseMsg);           // 序列化后发送
        myFunc_(resCodec.encodeMsg());
    }
}

void Communication::handleAesFenFa(Message *msg, Message &responseMsg) {
    Debug("Enter handleAesFenFa function");
    // 获取来自客户端的对称加密密钥，并校验
    RsaCrypto rsa;
    rsa.parseStringToKey(redis_->rsaSecKey("PrivateKey"), RsaCrypto::PrivateKey);
    aesKey_ = rsa.priKeyDecrypt(msg->data1);
    Hash hash(HashType::Sha224);
    hash.addData(aesKey_);

    // 回复消息
    if (msg->data2 != hash.result()) {
        Debug("Aes密钥 哈希校验失败");
        responseMsg.data1 = "Aes密钥 哈希校验失败";
        responseMsg.responseCode = ResponseCode::Failed;
    } else {
        Debug("Aes密钥 哈希校验成功");
        aes_ = new AesCrypto(AesCrypto::aes_256_cbc, aesKey_);
        responseMsg.responseCode = ResponseCode::AesVerifyOK;
    }
}

void Communication::setCallback(Communication::sendCallback sendFunc, Communication::deleteCallback deleteFunc) {
    sendFunc_ = sendFunc;
    deleteFunc_ = deleteFunc;
}

Communication::~Communication() {
    delete aes_;
    delete mysql_;
    delete redis_;
}

void Communication::handleRegister(Message *msg, Message &responseMsg) {
    Debug("Enter handleRegister function");
    char sql[1024];
    sprintf(sql, "SELECT * FROM `user` WHERE name = '%s'", msg->userName.data());
    bool flag = mysql_->query(sql);
    if (flag && !mysql_->next()) {
        // 将注册信息写入数据库
        mysql_->transaction();
        sprintf(sql, "INSERT INTO `user` (name, passwd, phone, date) VALUES ('%s', '%s', '%s', NOW())",
                msg->userName.data(), msg->data1.data(), msg->data2.data());
        bool fl1 = mysql_->update(sql);
        sprintf(sql, "INSERT INTO `information` (name, score, status) VALUES ('%s', 0, 0)",
                msg->userName.data());
        bool fl2 = mysql_->update(sql);
        if (fl1 && fl2) {
            mysql_->commit();
            responseMsg.responseCode = ResponseCode::RegisterOK;
        } else {
            mysql_->rollback();
            responseMsg.responseCode = ResponseCode::Failed;
            responseMsg.data1 = "数据库插入数据失败";
        }
    } else {
        responseMsg.responseCode = ResponseCode::Failed;
        responseMsg.data1 = "用户名已存在，注册失败";
    }
}

Communication::Communication() {
    JsonParse json;
    auto info = json.getDBInfo(JsonParse::Mysql);
    mysql_ = new MysqlConn;
    bool flag = mysql_->connect(info->user, info->passwd, info->dbname, info->ip, info->port);
    assert(flag);

    // 连接redis服务器
    redis_ = new Room();
    assert(redis_->initEnvironment());
}

void Communication::handleLogin(Message *msg, Message &responseMsg) {
    Debug("Enter handleLogin function");
    char sql[1024];
    // 查询该用户是否已注册且未登录
    sprintf(sql, "SELECT * FROM "
                 "(SELECT user.`name`, `user`.passwd, information.`status` "
                 "from `user` JOIN information ON user.`name` = information.`name`) as t "
                 "WHERE name = '%s' AND passwd = '%s' AND status = 0",
            msg->userName.data(), msg->data1.data());
    bool flag = mysql_->query(sql);
    if (flag && mysql_->next()) {
        // 更新登录状态
        mysql_->transaction();
        sprintf(sql, "UPDATE information SET status = 1 WHERE name = '%s'",
                msg->userName.data());
        bool flag1 = mysql_->update(sql);
        if (flag1) {  // 登录成功
            mysql_->commit();
            responseMsg.responseCode = ResponseCode::LoginOK;
        } else {      // 登录失败
            mysql_->rollback();
            responseMsg.responseCode = ResponseCode::Failed;
            responseMsg.data1 = "登录失败";
        }
    } else {
        responseMsg.responseCode = ResponseCode::Failed;
        responseMsg.data1 = "用户名或密码错误，或当前用户已登录";
    }
}

void Communication::handleAddRoom(Message *msg, Message &responseMsg) {
    std::string oldRoom = redis_->whereAmI(msg->userName);      // 当前玩家不是第一次加入房间(登录之后)
    int score = redis_->playerScore(oldRoom, msg->userName);    // 查询上次的房间并读出分数
    if (!oldRoom.empty()) {                             // 自动加入房间模式时再开新游戏会从旧房间退出并随机加入新房间
        redis_->quitRoom(oldRoom, msg->userName);               // redis部分 退出旧房间的处理
        RoomList::getInstance()->removePlayer(oldRoom, msg->roomName);  // 全局变量部分
    }
    bool flag = true;
    std::string roomName;
    if (msg->requestCode == RequestCode::AutoRoom) {
        roomName = redis_->joinRoom(msg->userName);
    } else if (msg->requestCode == RequestCode::ManualRoom) {
        roomName = msg->roomName;
        flag = redis_->joinRoom(msg->userName, msg->roomName);
    }

    // 判断是否已经加入某个房间，并回复消息
    if (flag) {
        // 第一次加载分数，在redis中更新分数，并最后同步到mysql
        if (score == 0) {       // 查询mysql，并将其存储到redis
            std::string sql = "SELECT score FROM information WHERE name = '" + msg->userName + "'";
            assert(mysql_->query(sql));
            mysql_->next();
            score = std::stoi(mysql_->value(0));
        }
        redis_->updatePlayerScore(roomName, msg->userName, score);  // 更新/添加 user到新房间set

        // 将房间和玩家的映射保存到单例RoomList
        RoomList *roomList = RoomList::getInstance();
        roomList->addUser(roomName, msg->userName, sendFunc_);

        // 回复数据
        responseMsg.responseCode = ResponseCode::JoinRoomOK;
        responseMsg.data1 = std::to_string(redis_->getPlayerCount(roomName));
        responseMsg.roomName = roomName;       // 自动创建时不知道房间名
    } else {
        responseMsg.responseCode = ResponseCode::Failed;
        responseMsg.data1 = "加入房间失败";
    }
}

void Communication::readyForPlay(const std::string &roomName, const std::string &data) {
    RoomList *roomList = RoomList::getInstance();
    UserMap players = roomList->getPlayers(roomName);

    for (const auto &iter: players) {
        Debug("send JoinRoomOK to user: %s", iter.first.data());
        iter.second(data);
    }
    if (players.size() == 3) {
        startGame(players, roomName);
    }
}

void Communication::dealCards(const UserMap &players) {
    Message msg;
    initCards();
    std::string all;
    for (int i = 0; i < 51; i++) {
        auto card = takeOneCard();
        std::string sub = std::to_string(card.first) + "-" + std::to_string(card.second) + "#";
        all += sub;
    }
    std::string lastCard;   // 剩余3张底牌
    for (const auto &iter: cards_) {
        std::string sub = std::to_string(iter.first) + "-" + std::to_string(iter.second) + "#";
        lastCard += sub;
    }
    msg.data1 = all;
    msg.data2 = lastCard;
    msg.responseCode = ResponseCode::DealCards;
    Codec codec(&msg);
    for (const auto &iter: players) {       // 将发牌信息传给房间内所有玩家
        iter.second(codec.encodeMsg());
    }
}

void Communication::initCards() {
    cards_.clear();
    for (int i = 1; i <= 4; i++) {          // 花色
        for (int j = 1; j <= 13; j++) {     // 点数
            cards_.insert(std::make_pair(i, j));
        }
    }
    cards_.insert(std::make_pair(0, 14));   // 大小王
    cards_.insert(std::make_pair(0, 15));
}

std::pair<int, int> Communication::takeOneCard() {
    std::random_device rand;            // 创建随机设备对象
    std::mt19937 gen(rand());           // 创建随机数生成对象
    std::uniform_int_distribution<int> dis(0, cards_.size() - 1);     // 创建随机数分布对象：均匀分布
    int randomNum = dis(gen);
    auto it = cards_.begin();
    for (int i = 0; i < randomNum; i++, it++);
    cards_.erase(it);
    return *it;
}

void
Communication::notifyOtherPlayers(const std::string &data, const std::string &roomName, const std::string &userName) {
    auto otherPlayers = RoomList::getInstance()->getPartners(roomName, userName);
    for (const auto &iter: otherPlayers) {
        iter.second(data);
    }
}

void Communication::restartGame(Message *msg) {
    UserMap players = RoomList::getInstance()->getPlayers(msg->roomName);
    if (players.size() == 3) {
        // 手动加入的再开游戏时需要3人都同意，此处用RoomList的UserMap作为计数
        RoomList::getInstance()->removeRoom(msg->roomName);
    }
    RoomList::getInstance()->addUser(msg->roomName, msg->userName, sendFunc_);
    players = RoomList::getInstance()->getPlayers(msg->roomName);
    if (players.size() == 3) {
        // 当得到3人同意再开时，进行重新发牌
        startGame(players, msg->roomName);
    }
}

void Communication::startGame(const UserMap& players, const std::string& roomName) {
    dealCards(players);
    Message msg;
    msg.responseCode = ResponseCode::StartGame;
    msg.data1 = redis_->playersOrder(roomName);
    Codec codec(&msg);
    for (const auto &iter: players) {       // 通知房间内所有玩家开始游戏
        Debug("send StartGame to user: %s", iter.first.data());
        iter.second(codec.encodeMsg());
    }
}

void Communication::handleLeaveRoom(Message *msg, Message &responseMsg) {
    redis_->quitRoom(msg->roomName, msg->userName);
    RoomList::getInstance()->removePlayer(msg->roomName, msg->userName);
    // 获取该房间剩下的其他玩家
    UserMap players = RoomList::getInstance()->getPlayers(msg->roomName);
    responseMsg.responseCode = ResponseCode::OtherLeaveRoom;
    responseMsg.data1 = std::to_string(players.size());
    // 数据转发
    for (const auto& iter: players) {
        Codec codec1(&responseMsg);
        iter.second(codec1.encodeMsg());
    }
}

void Communication::handleGoodbye(Message *msg) {
    char sql[128];
    sprintf(sql, "update information set status = 0 where name = '%s'", msg->userName.data());
    mysql_->update(sql);
    deleteFunc_();
}
