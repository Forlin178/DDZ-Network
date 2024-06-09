#include "WorkerThread.h"
#include <stdio.h>
#include <stdlib.h>

void WorkerThread::running() {
	mutex_.lock();
	evLoop_ = new EventLoop(threadName_);	// 创建evLoop
	mutex_.unlock();
	cond_.notify_one();						// 已创建evLoop实例，唤醒阻塞线程
	evLoop_->run();							// 启动子线程的EventLoop
}

WorkerThread::WorkerThread(int index)
{
	threadID_ = std::thread::id();
	thread_ = nullptr;
	evLoop_ = nullptr;
	threadName_ = "SubThread_" + std::to_string(index);
}

WorkerThread::~WorkerThread() {
	if (thread_ != nullptr)
		delete thread_;
}

void WorkerThread::run()
{// 主线程调用
	thread_ = new std::thread(&WorkerThread::running, this);

	// 主线程(workerThreadRun)和子线程(subThreadRunning)在共享thread->evLoop
	// 防止主线程调用workerThreadRun后使用空的evLoop
	// 避免在running new evLoop_ 之前使用空指针
	std::unique_lock<std::mutex> lck(mutex_);
	while (evLoop_ == nullptr) {
		cond_.wait(lck);
	}
}
