#include "Codec.h"
#include "TcpConnection.h"
#include "RsaCrypto.h"
#include "Room.h"
#include <string>
#include <fstream>
#include <sstream>

int TcpConnection::processRead(void* arg) {	// 读缓冲区有数据就一直触发
	auto* connect = static_cast<TcpConnection*>(arg);
	// 接收数据
	int count = connect->readBuf_->socketRead(connect->channel_->getSocket());
	if (count > 0) {
        // 解析斗地主数据
        connect->reply_->parseRequest(connect->readBuf_);
	} else {
        // 和客户端断开连接
        connect->addDeleteTask();
    }
	return 0;
}

// MSG_SEND_DATA被注释就等于没用
int TcpConnection::processWrite(void* arg) {	// 写缓冲区有空间就一直触发
	auto* connect = static_cast<TcpConnection*>(arg);
	int count = connect->writeBuf_->sendData(connect->channel_->getSocket());
	if (count > 0) {
		// 判断数据是否全部发送
		if (connect->writeBuf_->getWritePos() - connect->writeBuf_->getReadPos() == 0) {
            connect->channel_->setCurrentEvent(FDEvent::ReadEvent);
            connect->evLoop_->addTask(connect->channel_, ElemtType::MODIFY);
		}
	}
	return 0;
}


int TcpConnection::destroy(void* arg)
{
	auto* connect = static_cast<TcpConnection*>(arg);
	if (connect == nullptr) return 0;
	delete connect;
	return 0;
}

TcpConnection::TcpConnection(int fd, EventLoop* evLoop)
{
	evLoop_ = evLoop;
	readBuf_ = new Buffer(10240);
	writeBuf_ = new Buffer(10240);
    reply_ = new Communication();
    auto sendBind = std::bind(&TcpConnection::addWriteTask, this, std::placeholders::_1);
    auto deleteBind = std::bind(&TcpConnection::addDeleteTask, this);
    reply_->setCallback(sendBind, deleteBind);

//	request_ = new HttpRequest();  高性能服务器的上层已替换为斗地主
//	response_ = new HttpResponse();
	name_ = "Connection-" + std::to_string(fd);
    prepareSecretKey();
	channel_ = new Channel(fd, FDEvent::WriteEvent,
		processRead, processWrite, destroy, this);
	// 将tcp连接添加到子线程的evLoop，在子线程中处理http请求
	evLoop->addTask(channel_, ElemtType::ADD);
}

TcpConnection::~TcpConnection()
{
	if (readBuf_ && readBuf_->getWritePos() - readBuf_->getReadPos() == 0 &&
		writeBuf_ && writeBuf_->getWritePos() - writeBuf_->getReadPos() == 0) {
		delete readBuf_;
		delete writeBuf_;
		evLoop_->freeChannel(channel_);
	}
}

void TcpConnection::prepareSecretKey() {
    Room redis;
    redis.initEnvironment();
    std::string pubkey = redis.rsaSecKey("PublicKey");
    // 发送公钥
    Message msg;
    msg.responseCode = ResponseCode::RsaFenFa;      // RSA密钥的分发
    msg.data1 = pubkey;                               // data1：公钥
    RsaCrypto rsa;
    rsa.parseStringToKey(redis.rsaSecKey("PrivateKey"), RsaCrypto::PrivateKey);
    msg.data2 = rsa.sign(pubkey);                     // data2：公钥的签名
    Codec codec(&msg);
    auto serialized = codec.encodeMsg();
    // 写数据
    writeBuf_->appendPackage(serialized);
}

void TcpConnection::addWriteTask(const std::string& data) {
    writeBuf_->appendPackage(data);
#if 1   // 通过事件完成发送
    channel_->setCurrentEvent(FDEvent::WriteEvent);
    evLoop_->addTask(channel_, ElemtType::MODIFY);
#else   // 直接发送
    writeBuf_->sendData(channel_->getSocket());
#endif
}

void TcpConnection::addDeleteTask() {
    Debug("已断开和客户端的连接 name = %s", name_.data());
    evLoop_->addTask(channel_, ElemtType::DELETE);
}
