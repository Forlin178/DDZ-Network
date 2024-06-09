#pragma once
#include "Channel.h"
#include "EventLoop.h"
#include <string>

class EventLoop;

class Dispatcher
{
public:
	Dispatcher(EventLoop* evLoop);
	// 当父类指针需要指向子类时，需要将析构函数设为虚构，避免用父类析构子类
	virtual ~Dispatcher();
	// 添加
	virtual int add();
	// 删除
	virtual int remove();
	// 修改
	virtual int modify();
	// 事件监测(单位:s)
	virtual int dispatch(int timeout = 2);

	inline void setChannel(Channel* channel) { this->channel_ = channel; }

protected:
	std::string name_=std::string();
	Channel* channel_;
	EventLoop* evLoop_;
};