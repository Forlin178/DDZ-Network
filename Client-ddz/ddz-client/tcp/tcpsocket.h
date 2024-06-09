#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <qglobal.h>
#include <QObject>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);
    explicit TcpSocket(QByteArray ip, unsigned short port, QObject *parent = nullptr);
    ~TcpSocket();

    // 链接服务器
    bool connectToHost(QByteArray ip, unsigned short port);
    // 接收数据（单位：秒）
    QByteArray recvMsg(int timeout = -1);
    // 发送数据
    bool sendMsg(QByteArray msg, int timeout = -1);
    // 断开连接
    void disconnect();

private:
    bool readTimeout(int timeout);
    bool writeTimeout(int timeout);
    int readn(char *buf, int count);
    int writen(const char *buf, int count);

signals:

private:
#ifdef Q_OS_WIN
    SOCKET socket_;
#endif
#ifdef Q_OS_LINUX
    int socket_;
#endif
};

#endif // TCPSOCKET_H
