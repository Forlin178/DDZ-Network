#pragma once
#include <functional>

// typedef int(*handleFunc)(void* arg);
// using handleFunc = int(*)(void*);

enum class FDEvent {
	TimeOut = 0x01,
	ReadEvent = 0x02,
	WriteEvent=0x04
};

class Channel
{
public:
	using handleFunc = std::function<int(void*)>;

	Channel(int fd, FDEvent events, handleFunc readCallBack,
		handleFunc writeCallBack, handleFunc destroyCallBack, void* arg);

	handleFunc readCallBack;	// 读回调函数
	handleFunc writeCallBack;	// 写回调函数
	handleFunc destroyCallBack;	// 析构回调函数

	// 修改fd的写事件(检测 | 不检测)
	void writeEventEnable(bool isEnable);
    // 设置当前要检测的事件
    void setCurrentEvent(FDEvent event);

	// 判断是否需要检测文件描述符的写事件
	bool isWriteEventEnable();
    bool isReadEventEnable();

	inline int getEvent() { return events_; }
	inline int getSocket() { return fd_; }
	inline const void* getArg() { return arg_; }

private:
	int fd_;						// 文件描述符
	int events_;					// 需要检测的事件(read/write/rw)
	void* arg_;					// 回调函数的参数
};

