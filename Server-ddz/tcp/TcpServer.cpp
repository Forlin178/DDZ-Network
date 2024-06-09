#include "TcpConnection.h"
#include "TcpServer.h"
#include "RsaCrypto.h"
#include "Log.h"
#include "Room.h"
#include <arpa/inet.h>
#include <fstream>
#include <sstream>
#include <csignal>

int TcpServer::acceptConnection(void* arg) {
	// 在主线程中接受客户端的连接请求，并将后续客户端请求交给子线程处理
	auto* server = static_cast<TcpServer*>(arg);
	int cfd = accept(server->lfd_, NULL, NULL);
	EventLoop* evLoop = server->threadPool_->takeWorkerEventLoop();
	auto* connect = new TcpConnection(cfd, evLoop);
	return 0;
}


TcpServer::TcpServer(unsigned short port, int threadNum)
{
	port_ = port;
	threadNum_ = threadNum;
	mainLoop_ = new EventLoop();
	threadPool_ = new ThreadPool(mainLoop_, threadNum);
	setListen();
}

TcpServer::~TcpServer() {}

void TcpServer::setListen()
{
	// 1.创建用于监听的文件描述符
	lfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd_ == -1) {
		perror("socket");
		return;
	}
	int opt = 1;
	int ret = setsockopt(lfd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret == -1) {
		perror("setsocketopt");
		return;
	}
	// 2.绑定本地IP和端口
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	addr.sin_addr.s_addr = INADDR_ANY;
	ret = bind(lfd_, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1) {
		perror("bind");
		return;
	}
	// 3.设置监听
	ret = listen(lfd_, 128);
	if (ret == -1) {
		perror("listen");
		return;
	}
}

void TcpServer::run()
{
    saveRsaKey();
	threadPool_->run();											// 启动线程池(启动子线程的反应堆)
	Channel* channel = new Channel(lfd_, FDEvent::ReadEvent,
		acceptConnection, NULL, NULL, this);					// 监听fd的封装
	mainLoop_->addTask(channel, ElemtType::ADD);		        // 给主反应堆添加检测任务
	mainLoop_->run();											// 启动主线程反应堆
}

void TcpServer::saveRsaKey() {
    // 生成非对称加密的密钥对
    RsaCrypto rsaCrypto;
    rsaCrypto.generateRsaKey(RsaCrypto::BITS_2K);

    // 读公钥文件
    std::ifstream ifs("public.pem");
    std::stringstream sstr;
    sstr << ifs.rdbuf();
    auto data = sstr.str();
    ifs.close();

    // 创建redis对象
    Room redis{};
    assert(redis.initEnvironment());
    redis.clear();
    redis.saveRsaSecKey("PublicKey", data);

    // 读私钥文件
    ifs.open("private.pem");
    sstr << ifs.rdbuf();
    data = sstr.str();
    ifs.close();
    redis.saveRsaSecKey("PrivateKey", data);

    // 删除对应磁盘文件
    unlink("public.pem");
    unlink("private.pem");
}
