#include "EpollDispatcher.h"
#include "EventLoop.h"
#include "PollDispatcher.h"
#include "SelectDispatcher.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

EventLoop::EventLoop()
	: EventLoop(std::string())
{}

EventLoop::EventLoop(const std::string threadName)
{
	isQuit_ = true;
	threadID_ = std::this_thread::get_id();
	threadName_ = threadName == std::string() ? "MainThread" : threadName;

	// dispatcher
	dispatcher_ = new EpollDispatcher(this);

	// map
	channelMap_.clear();

	// socketPair
	int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, socketPair_);
	if (ret == -1) {
		perror("socketpair");
		exit(0);
	}
#if 0
	Channel* channel = new Channel(socketPair_[1], FDEvent::ReadEvent,
		readLocalMessage, nullptr, nullptr, this);
#else
	auto obj = std::bind(&EventLoop::readMessage, this);
	Channel* channel = new Channel(socketPair_[1], FDEvent::ReadEvent,
		obj, nullptr, nullptr, this);
#endif
	addTask(channel, ElemtType::ADD);
}

EventLoop::~EventLoop() {}

int EventLoop::run()
{
	isQuit_ = false;
	if (threadID_ != std::this_thread::get_id())
		return -1;

	// 循环进行事件处理
	while (!isQuit_)
	{
		dispatcher_->dispatch();	// 超时时长: 2s
		processTask();
	}
	return 0;
}

int EventLoop::active(int fd, int event)
{
	// event表示fd触发的事件，fd可能检测的是rw，但不一定都触发
	// 所以判断条件不能用 channel->events 代替 event
	// 要交由dispatcher->dispatch判断
	if (fd < 0)
		return -1;

	// 取出channel
	Channel* channel = channelMap_[fd];
	assert(channel->getSocket() == fd);

	if (event & (int)FDEvent::ReadEvent && channel->readCallBack) {
		channel->readCallBack(const_cast<void*>(channel->getArg()));
	}
	if (event & (int)FDEvent::WriteEvent && channel->writeCallBack) {
		channel->writeCallBack(const_cast<void*>(channel->getArg()));
	}
	return 0;
}

int EventLoop::addTask(Channel* channel, ElemtType type)
{
	mutex_.lock();	// 保护任务队列资源
	struct ChannelElement newElement;
	newElement.channel = channel;
	newElement.type = type;
	taskQueue_.push(newElement);
	mutex_.unlock();

	/*
	* 处理节点：
	* 1.对于链表节点的添加：可能是当前线程也可能是其他线程(主线程)
	*	1) type=modify 由当前子线程添加，由当前子线程处理
	*	2) type=add    建立连接任务由主线程添加
	* 2.不能让主线程处理任务队列，需由当前子线程处理
	*/
	if (threadID_ == std::this_thread::get_id()) {
		processTask();
	}
	else {
		// 1.子线程在processTask(无事) 2.子线程阻塞在dispatch(需主动触发事件以提前结束)
		taskWakeUp();
	}

	return 0;
}

int EventLoop::processTask()
{
	
	ChannelElement head;
	while (!taskQueue_.empty()) {
		mutex_.lock();
		head = taskQueue_.front();
		taskQueue_.pop();
		mutex_.unlock();
		Channel* channel = head.channel;
		if (head.type == ElemtType::ADD) {
			add(channel);
		}
		else if (head.type == ElemtType::DELETE) {
			remove(channel);
		}
		else if (head.type == ElemtType::MODIFY) {
			modify(channel);
		}
	}
	return 0;
}

int EventLoop::add(Channel* channel)
{// 将任务队列中的任务添加到 map 和 dispatcher 检测集合中
	int fd = channel->getSocket();
	if (channelMap_.count(fd) == 0) {
		channelMap_.insert(std::pair<int, Channel*>(fd, channel));
		dispatcher_->setChannel(channel);
		int ret = dispatcher_->add();
		return ret;
	}
	return -1;
}

int EventLoop::remove(Channel* channel)
{
	int fd = channel->getSocket();
	if (channelMap_.count(fd) == 0)	return -1;
	dispatcher_->setChannel(channel);
	int ret = dispatcher_->remove();
	channel->destroyCallBack(const_cast<void*>(channel->getArg()));
	return ret;
}

int EventLoop::modify(Channel* channel)
{
	int fd = channel->getSocket();
	if (channelMap_.count(fd) == 0)
		return -1;
	dispatcher_->setChannel(channel);
	int ret = dispatcher_->modify();
	return ret;
}

int EventLoop::readLocalMessage(void* arg)
{// 静态成员函数不能直接访问非静态成员变量
 // 但可以传递实例，通过实例访问(private也可以)
	EventLoop* evLoop = static_cast<EventLoop*>(arg);
	char buf[128];
	read(evLoop->socketPair_[1], buf, sizeof(buf));
	return 0;
}

int EventLoop::freeChannel(Channel* channel)
{
	auto iter = channelMap_.find(channel->getSocket());
	if (iter != channelMap_.end()) {
		channelMap_.erase(channel->getSocket());
		close(channel->getSocket());
		delete channel;
	}
	return 0;
}

int EventLoop::readMessage()
{
	char buf[128];
	read(socketPair_[1], buf, sizeof(buf));
	return 0;
}

void EventLoop::taskWakeUp()
{
	const char* msg = "Forlin";
	write(socketPair_[0], msg, strlen(msg));
}
