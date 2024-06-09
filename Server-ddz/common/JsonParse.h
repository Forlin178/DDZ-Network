//
// Created by 38491 on 2024/4/15.
//

#ifndef SERVER_DDZ_JSONPARSE_H
#define SERVER_DDZ_JSONPARSE_H

#include <string>
#include <memory>
#include <json/json.h>

struct DBInfo {
    std::string ip;
    unsigned short port;
    std::string user;
    std::string passwd;
    std::string dbname;
};

class JsonParse {
public:
    enum DBType { Mysql, Redis };
    JsonParse(std::string fileName = "../config/config.json");
    // 获取数据
    std::shared_ptr<DBInfo> getDBInfo(DBType type);

private:
    Json::Value root_;
};


#endif //SERVER_DDZ_JSONPARSE_H
