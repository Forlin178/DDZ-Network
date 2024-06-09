#pragma once
#include "EventLoop.h"
#include "ThreadPool.h"

class TcpServer
{
public:
	TcpServer(unsigned short port, int threadNum);
	~TcpServer();

	void setListen();	// 初始化监听
	void run();			// 启动服务器

    void saveRsaKey();  // 存储rsa密钥

	static int acceptConnection(void* arg);

private:
	int threadNum_;
	EventLoop* mainLoop_;
	ThreadPool* threadPool_;
	int lfd_;
	unsigned short port_;
};


