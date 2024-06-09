//
// Created by 38491 on 2024/4/12.
//

#include "MysqlConn.h"

MysqlConn::MysqlConn() {
    connect_ = mysql_init(nullptr);
    mysql_set_character_set(connect_, "utf8");
}

MysqlConn::~MysqlConn() {
    if (connect_)
        mysql_close(connect_);
    freeResult();
}

bool MysqlConn::connect(std::string user, std::string passwd, std::string dbName, std::string ip, unsigned short port) {
    auto ptr = mysql_real_connect(connect_, ip.data(), user.data(), passwd.data(), dbName.data(), port, nullptr, 0);
    return ptr != nullptr;
}

bool MysqlConn::update(std::string sql) {
    return mysql_query(connect_, sql.data()) == 0;
}

bool MysqlConn::query(std::string sql) {
    freeResult();       // 释放上次的查询结果
    if (mysql_query(connect_, sql.data()))
        return false;
    result_ = mysql_store_result(connect_);
    return result_ != nullptr;
}

bool MysqlConn::next() {
    if (result_) {
        row_ = mysql_fetch_row(result_);
        return row_ != nullptr;
    }
    return false;
}

std::string MysqlConn::value(int index) {
    int colNum = mysql_num_fields(result_);
    auto lengths = mysql_fetch_lengths(result_);
    if (index < 0 || index >= colNum) return std::string();
    return std::string(row_[index], lengths[index]);
}

void MysqlConn::transaction() {
    // transaction ... ... commit/rollback
    mysql_autocommit(connect_, false);   // 改用手动提交
}

void MysqlConn::commit() {
    mysql_commit(connect_);
    mysql_autocommit(connect_, true);    // 改回自动挡
}

void MysqlConn::rollback() {
    mysql_rollback(connect_);
    mysql_autocommit(connect_, true);    // 改回自动挡
}

void MysqlConn::freeResult() {
    if (result_) {
        mysql_free_result(result_);
        result_ = nullptr;
    }
}
