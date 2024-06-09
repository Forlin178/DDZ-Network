//
// Created by 38491 on 2024/4/15.
//

#include <fstream>
#include <cassert>
#include "JsonParse.h"

JsonParse::JsonParse(std::string fileName) {
    // 打开文件
    std::ifstream ifs(fileName);
    assert(ifs.is_open());

    // 读取数据
    Json::Reader rd;
    rd.parse(ifs, root_);
    assert(root_.isObject());
}

std::shared_ptr<DBInfo> JsonParse::getDBInfo(JsonParse::DBType type) {
    std::string dbname = type == Mysql ? "mysql" : "redis";
    Json::Value node = root_[dbname];
    auto info = std::make_shared<DBInfo>();
    info->ip = node["ip"].asString();
    info->port = node["port"].asInt();
    if (type == Mysql) {
        info->user = node["user"].asString();
        info->passwd = node["password"].asString();
        info->dbname = node["dbname"].asString();
    }
    return info;
}
