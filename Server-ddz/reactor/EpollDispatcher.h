#pragma once
#include "Channel.h"
#include "Dispatcher.h"
#include "EventLoop.h"
#include <string>
#include <sys/epoll.h>

class EventLoop;
class EpollDispatcher : public Dispatcher {
public:
	EpollDispatcher(EventLoop* evLoop);
	~EpollDispatcher();
	// 添加
	int add();
	// 删除
	int remove();
	// 修改
	int modify();
	// 事件监测(单位:s)
	int dispatch(int timeout = 2);

private:
	int epollClt(int op);

private:
	int epfd_;
	struct epoll_event* evs_;	// epoll_wait中返回激活事件的数组
	const int maxNode_{ 520 };
};