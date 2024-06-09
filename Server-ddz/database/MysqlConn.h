//
// Created by 38491 on 2024/4/12.
//

#ifndef SERVER_DDZ_MYSQLCONN_H
#define SERVER_DDZ_MYSQLCONN_H

#include <mysql.h>
#include <string>

class MysqlConn {
public:
    MysqlConn();

    ~MysqlConn();

    // 连接数据库
    bool connect(std::string user, std::string passwd, std::string dbName,
                 std::string ip, unsigned short port = 3306);

    // 更新数据库: insert/update/delete
    bool update(std::string sql);

    // 查询数据库
    bool query(std::string sql);

    // 遍历查询得到的结果集
    bool next();

    // 得到结果集中的字段值
    std::string value(int index);

    // 事务操作(取消autocommit)
    void transaction();

    // 提交事务
    void commit();

    // 事务回滚
    void rollback();

private:
    void freeResult();

private:
    MYSQL *connect_{nullptr};
    MYSQL_RES *result_{nullptr};
    MYSQL_ROW row_;
};


#endif //SERVER_DDZ_MYSQLCONN_H
