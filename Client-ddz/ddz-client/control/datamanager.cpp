#include "datamanager.h"

// 饿汉单例(开始就初始化)
DataManager* DataManager::data_ = new DataManager;      // 初始化单例

DataManager *DataManager::getInstance()
{
    return data_;
}

QByteArray DataManager::userName() const
{
    return userName_;
}

void DataManager::setUserName(const QByteArray &newUserName)
{
    userName_ = newUserName;
}

QByteArray DataManager::ip() const
{
    return ip_;
}

void DataManager::setIp(const QByteArray &newIp)
{
    ip_ = newIp;
}

QByteArray DataManager::port() const
{
    return port_;
}

void DataManager::setPort(const QByteArray &newPort)
{
    port_ = newPort;
}

Communication *DataManager::communication() const
{
    return communication_;
}

void DataManager::setCommunication(Communication *newCommunication)
{
    communication_ = newCommunication;
}

QByteArray DataManager::roomName() const
{
    return roomName_;
}

void DataManager::setRoomName(const QByteArray &newRoomName)
{
    roomName_ = newRoomName;
}

Cards DataManager::cards() const
{
    return cards_;
}

void DataManager::setCards(const Cards &newCards)
{
    cards_ = newCards;
}

Cards DataManager::last() const
{
    return last_;
}

void DataManager::setLast(const Cards &newLast)
{
    last_ = newLast;
}

DataManager::GameMode DataManager::mode() const
{
    return mode_;
}

void DataManager::setMode(GameMode newMode)
{
    mode_ = newMode;
}

DataManager::RoomMode DataManager::roomMode() const
{
    return roomMode_;
}

void DataManager::setRoomMode(RoomMode newRoomMode)
{
    roomMode_ = newRoomMode;
}

