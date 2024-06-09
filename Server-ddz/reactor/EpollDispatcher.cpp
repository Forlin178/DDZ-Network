#include "EpollDispatcher.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


EpollDispatcher::EpollDispatcher(EventLoop* evLoop)
	: Dispatcher(evLoop)
{
	epfd_ = epoll_create(1);
	if (epfd_ == -1) {
		perror("epoll_create");
		exit(0);
	}
	evs_ = new struct epoll_event[maxNode_];
	name_ = "Epoll";
}

EpollDispatcher::~EpollDispatcher()
{
	close(epfd_);
	delete[]evs_;
}

int EpollDispatcher::add()
{
	int ret = epollClt(EPOLL_CTL_ADD);
	if (ret == -1) {
		perror("epoll_ctl add");
		exit(0);
	}
	return ret;
}

int EpollDispatcher::remove()
{
	int ret = epollClt(EPOLL_CTL_DEL);
	if (ret == -1) {
		perror("epoll_ctl del");
		exit(0);
	}
	return ret;
}

int EpollDispatcher::modify()
{
	int ret = epollClt(EPOLL_CTL_MOD);
	if (ret == -1) {
		perror("epoll_ctl mod");
		exit(0);
	}
	return ret;
}

int EpollDispatcher::dispatch(int timeout)
{
	int count = epoll_wait(epfd_, evs_, maxNode_, timeout * 1000);
	for (int i = 0; i < count; i++) {
		int fd = evs_[i].data.fd;
		int events = evs_[i].events;
		if (events & EPOLLERR || events & EPOLLHUP) {
			// 对方断开连接
			// epollRemove
			continue;
		}
		if (events & EPOLLIN) {
			evLoop_->active(fd, (int)FDEvent::ReadEvent);
		}
		if (events & EPOLLOUT) {
			evLoop_->active(fd, (int)FDEvent::WriteEvent);
		}
	}
	return 0;
}

int EpollDispatcher::epollClt(int op)
{
	// channel: fd(操作对象), events(需监听的事件)
	// EventLoop: dispatcherData->epfd(epoll树根节点)
	struct epoll_event ev;
	ev.data.fd = channel_->getSocket();
	int events = 0;
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		events |= EPOLLIN;
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		events |= EPOLLOUT;
	ev.events = events;

	int ret = epoll_ctl(epfd_, op, channel_->getSocket(), &ev);
	return ret;
}
