#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include "PollDispatcher.h"

PollDispatcher::PollDispatcher(EventLoop* evLoop)
	: Dispatcher(evLoop)
{
	maxfd_ = 0;
	fds_ = new struct pollfd[maxNode_];
	for (int i = 0; i < maxNode_; i++) {
		fds_[i].fd = -1;
		fds_[i].events = 0;
		fds_[i].revents = 0;
	}
	name_ = "Poll";
}

PollDispatcher::~PollDispatcher()
{
	delete[]fds_;
}

int PollDispatcher::add()
{
	short events = 0;
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		events |= POLLIN;
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		events |= POLLOUT;

	for (int i = 0; i < maxNode_; i++) {
		if (fds_[i].fd == -1) {
			fds_[i].fd = channel_->getSocket();
			fds_[i].events = events;
			maxfd_ = maxfd_ < i ? i : maxfd_;
			return 0;
		}
	}
	return -1;
}

int PollDispatcher::remove()
{
	for (int i = 0; i < maxNode_; i++) {
		if (fds_[i].fd == channel_->getSocket()) {
			fds_[i].fd = -1;
			fds_[i].events = 0;
			fds_[i].revents = 0;
			// data->maxfd = data->maxfd == i ? i-1 : data->maxfd;
			return 0;
		}
	}
	return -1;
}

int PollDispatcher::modify()
{
	short events = 0;
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		events |= POLLIN;
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		events |= POLLOUT;

	for (int i = 0; i < maxNode_; i++) {
		if (fds_[i].fd == channel_->getSocket()) {
			fds_[i].events = events;
			return 0;
		}
	}
	return -1;
}

int PollDispatcher::dispatch(int timeout)
{
	int ret = poll(fds_, maxfd_ + 1, timeout * 1000);
	if (ret == -1) {
		perror("poll");
		exit(0);
	}

	for (int i = 0; i <= maxfd_; i++) {
		if (fds_[i].fd != -1) {
			if (fds_[i].revents & POLLIN) {
				evLoop_->active(fds_[i].fd, (int)FDEvent::ReadEvent);
			}
			if (fds_[i].revents & POLLOUT) {
				evLoop_->active(fds_[i].fd, (int)FDEvent::WriteEvent);
			}
		}
	}
	return 0;
}
