#pragma once
#include "EventLoop.h"
#include "WorkerThread.h"
#include <vector>

// 线程池定义
class ThreadPool
{
public:
	ThreadPool(EventLoop* mainLoop, int count);
	~ThreadPool();

	void run();							// 启动线程池
	EventLoop* takeWorkerEventLoop();	// 取出线程池中子线程的反应堆实例

private:
	EventLoop* mainLoop_;
	bool isStart_;
	int threadNum_;
	std::vector<WorkerThread*> workerThreads_;			// 子线程数组
	int index_;
};

