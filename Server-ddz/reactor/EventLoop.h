#pragma once
#include "Channel.h"
#include "Dispatcher.h"
#include "Log.h"
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// 处理节点中channel的方式，ChannelElement type
enum class ElemtType :char
{
	ADD,
	DELETE,
	MODIFY
};

struct ChannelElement
{
	ElemtType type;	// add/delete/modify
	Channel* channel;
};

class Dispatcher;

class EventLoop
{
public:
	EventLoop();		// 主线程用第一个，子线程用第二个带名字
	EventLoop(const std::string threadName);
	~EventLoop();

	inline std::thread::id getThreadID() { return threadID_; }

	// 启动反应堆模型
	int run();

	// 处理激活的文件描述符
	int active(int fd, int event);

	// 添加任务到任务队列
	int addTask(struct Channel* channel, ElemtType type);

	// 处理任务队列中的任务
	int processTask();

	// 处理dispatcher中的节点，对map进行增删查改
	int add(Channel* channel);
	int remove(Channel* channel);
	int modify(Channel* channel);

	// 释放channel
	int freeChannel(Channel* channel);

	int readMessage();
	static int readLocalMessage(void* arg);

private:
	void taskWakeUp();


private:
	bool isQuit_;
	Dispatcher* dispatcher_;				// 指向子类的指针
	
	std::queue<ChannelElement> taskQueue_;	// 任务队列
	
	std::map<int, Channel*> channelMap_;	// map

	// 线程id name mutex
	std::thread::id threadID_;
	std::string threadName_;
	std::mutex mutex_;			// 为任务队列提供线程安全

	int socketPair_[2];			// 存储本地通信的fd，socketPair[0]发送数据 socketPair[1]读取数据
};
