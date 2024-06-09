#pragma once
#include "Channel.h"
#include "Dispatcher.h"
#include "EventLoop.h"
#include <poll.h>
#include <string>

class EventLoop;
class PollDispatcher : public Dispatcher {
public:
	PollDispatcher(EventLoop* evLoop);
	~PollDispatcher();
	// 添加
	int add();
	// 删除
	int remove();
	// 修改
	int modify();
	// 事件监测(单位:s)
	int dispatch(int timeout = 2);

private:
	int maxfd_;		// fds数组有效的最右边下标
	struct pollfd* fds_;
	const int maxNode_{ 1024 };
};