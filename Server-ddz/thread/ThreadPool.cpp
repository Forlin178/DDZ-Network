#include "ThreadPool.h"
#include "Log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

ThreadPool::ThreadPool(EventLoop* mainLoop, int count)
{
	mainLoop_ = mainLoop;
	threadNum_ = count;
	index_ = 0;
	isStart_ = false;
	workerThreads_.clear();
}

ThreadPool::~ThreadPool() {}

void ThreadPool::run()
{
	assert(!isStart_);
	// 不是主线程调用
	if (mainLoop_->getThreadID() != std::this_thread::get_id())
		exit(0);

	// 启动线程池
	isStart_ = true;
	if (threadNum_ > 0) {
		for (int i = 0; i < threadNum_; i++) {
			WorkerThread* tmp = new WorkerThread(i);
			tmp->run();
			workerThreads_.push_back(tmp);
		}
	}
}

EventLoop* ThreadPool::takeWorkerEventLoop()
{
	assert(isStart_);
	if (mainLoop_->getThreadID() != std::this_thread::get_id())		// 不是主线程调用
		exit(0);

	EventLoop* evLoop = mainLoop_;
	if (threadNum_ > 0) {
		evLoop = workerThreads_[index_]->getEventLoop();
		index_ = (index_ + 1) % threadNum_;
	}
	return evLoop;
}
