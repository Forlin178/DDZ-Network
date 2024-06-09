#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include "EventLoop.h"

// 子线程结构体
class WorkerThread
{
public:
	// index: 线程池中的索引
	WorkerThread(int index);
	~WorkerThread();

	inline EventLoop* getEventLoop() { return evLoop_; }

	void run();							// 启动线程

private:
	void running();						// 子线程回调函数

private:
	std::thread* thread_;				// 线程指针
    std::thread::id threadID_;			// 线程ID
    std::string threadName_;			// 线程名字
    std::mutex mutex_;					// 互斥锁
    std::condition_variable cond_;		// 条件变量
	EventLoop* evLoop_;					// 反应堆模型
};
