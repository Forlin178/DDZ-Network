#pragma once
#include "Channel.h"
#include "Dispatcher.h"
#include "EventLoop.h"
#include <string>
#include <sys/select.h>

class EventLoop;
class SelectDispatcher : public Dispatcher {
public:
	SelectDispatcher(EventLoop* evLoop);
	~SelectDispatcher();
	// 添加
	int add();
	// 删除
	int remove();
	// 修改
	int modify();
	// 事件监测(单位:s)
	int dispatch(int timeout = 2);

private:
	fd_set readfds_;
	fd_set writefds_;
	const int maxNode_{ 1024 };
};