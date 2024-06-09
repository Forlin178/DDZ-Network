#include "tcpsocket.h"
#include <QDebug>

TcpSocket::TcpSocket(QObject *parent)
    : QObject{parent}
{
#ifdef Q_OS_WIN
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);
#endif
}

TcpSocket::TcpSocket(QByteArray ip, unsigned short port, QObject *parent)
    : TcpSocket(parent)
{
    connectToHost(ip, port);
}

TcpSocket::~TcpSocket()
{
#ifdef Q_OS_WIN
    WSACleanup();
#endif
}

bool TcpSocket::connectToHost(QByteArray ip, unsigned short port)
{
    assert(port > 0 && port <= 65535);
    socket_ = socket(AF_INET, SOCK_STREAM, 0);
    assert(socket_);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.S_un.S_addr = inet_addr(ip.data());
    int ret = ::connect(socket_, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    return ret == 0;    // ret == -1 时建立连接失败
}

QByteArray TcpSocket::recvMsg(int timeout)
{
    QByteArray array;
    bool flag = readTimeout(timeout);
    if (flag) {
        // 接收数据 = 数据头(4B) + 数据块(msg.size)           避免数据粘包
        int headLen = 0;
        int ret = readn((char*)&headLen, sizeof(int));     // 读4B的数据头
        if (ret == 0)   // 断开连接了
            return QByteArray();

        qDebug() << "大端数据: " << headLen;
        headLen = ntohl(headLen);         // 32bit 网络字节序 -> 主机字节序
        qDebug() << "小端数据: " << headLen;

        char *data = new char[headLen];
        assert(data);
        ret = readn(data, headLen);
        if (ret == headLen)
            array = QByteArray(data, headLen);

        delete[]data;
    }
    return array;
}

bool TcpSocket::sendMsg(QByteArray msg, int timeout)
{
    bool flag = writeTimeout(timeout);
    if (flag) {
        // 发送数据 = 数据头(4B) + 数据块(msg.size)
        int headLen = htonl(msg.size());   // 32bit 主机字节序 -> 网络字节序

        int length = sizeof(int) + msg.size();
        char *data = new char[length];
        assert(data);
        memcpy(data, &headLen, sizeof(int));
        memcpy(data + sizeof(int), msg.data(), msg.size());

        int ret = writen(data, length);
        flag = ret == length ? true : false;

        delete[]data;
    }
    return flag;
}

void TcpSocket::disconnect()
{
    if (socket_) {
#ifdef Q_OS_WIN
        closesocket(socket_);
#endif
#ifdef Q_OS_LINUX
        close(socket_);
#endif
    }
}

bool TcpSocket::readTimeout(int timeout)
{   // select poll epoll中只有select是跨平台的
    if (timeout == -1)  return true;    // 可以读，可能阻塞读

#ifdef Q_OS_WIN
    int nfds = 0;
#endif
#ifdef Q_OS_LINUX
    int nfds = socket_ + 1;
#endif
    fd_set fdread;
    FD_ZERO(&fdread);
    FD_SET(socket_, &fdread);
    timeval t;
    t.tv_sec = timeout;
    t.tv_usec = 0;
    int ret = select(nfds, &fdread, NULL, NULL, &t);      // 阻塞 监听socket读事件 t秒
    return ret > 0;
}

bool TcpSocket::writeTimeout(int timeout)
{
    if (timeout == -1)  return true;    // 可以写，可能阻塞

#ifdef Q_OS_WIN
    int nfds = 0;
#endif
#ifdef Q_OS_LINUX
    int nfds = socket_ + 1;
#endif
    fd_set fdwrite;
    FD_ZERO(&fdwrite);
    FD_SET(socket_, &fdwrite);
    timeval t;
    t.tv_sec = timeout;
    t.tv_usec = 0;
    int ret = select(nfds, NULL, &fdwrite, NULL, &t);     // 阻塞 监听socket写事件 t秒
    return ret > 0;
}

int TcpSocket::readn(char *buf, int count)
{
    int last = count;       // 剩余字节数
    int size = 0;           // 每次读出的字节数
    char *ptr = buf;
    while (last > 0) {
        if ((size = recv(socket_, ptr, last, 0)) < 0) {
            perror("recv");
            return -1;
        }
        else if (size == 0) {
            break;      // 已断开连接
        }
        ptr += size;
        last -= size;
    }
    return count - last;    // 总共读出的字节数
}

int TcpSocket::writen(const char *buf, int count)
{
    int last = count;       // 剩余字节数
    int size = 0;           // 每次写入的字节数
    const char *ptr = buf;
    while (last > 0) {
        if ((size = send(socket_, ptr, last, 0)) < 0) {
            perror("send");
            return -1;
        }
        else if (size == 0) {
            continue;
        }
        ptr += size;
        last -= size;
    }
    return count - last;    // 总共写入/发送的字节数
}
