#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QByteArray>
#include "cards.h"
#include "communication.h"

// 饿汉单例(对全局变量的包装？)
class DataManager
{
public:
    enum GameMode {
        Single,
        Network
    };

    enum RoomMode {
        Auto,
        Manual
    };

    DataManager(const DataManager&) = delete;               // 禁用拷贝构造
    DataManager& operator=(const DataManager&) = delete;    // 禁用赋值运算

    static DataManager* getInstance();                      // 获取全局唯一的实例对象

    // userName
    QByteArray userName() const;
    void setUserName(const QByteArray &newUserName);
    // ip
    QByteArray ip() const;
    void setIp(const QByteArray &newIp);
    // port
    QByteArray port() const;
    void setPort(const QByteArray &newPort);

    Communication *communication() const;
    void setCommunication(Communication *newCommunication);

    QByteArray roomName() const;
    void setRoomName(const QByteArray &newRoomName);

    Cards cards() const;
    void setCards(const Cards &newCards);

    Cards last() const;
    void setLast(const Cards &newLast);

    GameMode mode() const;
    void setMode(GameMode newMode);

    RoomMode roomMode() const;
    void setRoomMode(RoomMode newRoomMode);

private:
    DataManager() = default;
    static DataManager *data_;

    QByteArray userName_;
    QByteArray roomName_;
    QByteArray ip_{"192.168.21.132"};
    QByteArray port_{"10000"};
    Communication *communication_{ nullptr };

    Cards cards_;
    Cards last_;

    GameMode mode_;
    RoomMode roomMode_;
};

#endif // DATAMANAGER_H
