#include <sys/select.h>
#include <stdio.h>
#include <stdlib.h>
#include "SelectDispatcher.h"


SelectDispatcher::SelectDispatcher(EventLoop* evLoop)
	: Dispatcher(evLoop)
{
	FD_ZERO(&readfds_);
	FD_ZERO(&writefds_);
	name_ = "Select";
}

SelectDispatcher::~SelectDispatcher()
{}

int SelectDispatcher::add()
{
	if (channel_->getSocket() >= maxNode_)
		return -1;
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		FD_SET(channel_->getSocket(), &readfds_);
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		FD_SET(channel_->getSocket(), &writefds_);
	return 0;
}

int SelectDispatcher::remove()
{
	if (channel_->getSocket() >= maxNode_)
		return -1;
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		FD_CLR(channel_->getSocket(), &readfds_);
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		FD_CLR(channel_->getSocket(), &writefds_);
	return 0;
}

int SelectDispatcher::modify()
{
	if (channel_->getSocket() >= maxNode_)
		return -1;

	FD_CLR(channel_->getSocket(), &readfds_);
	FD_CLR(channel_->getSocket(), &writefds_);
	if (channel_->getEvent() & (int)FDEvent::ReadEvent)
		FD_SET(channel_->getSocket(), &readfds_);
	if (channel_->getEvent() & (int)FDEvent::WriteEvent)
		FD_SET(channel_->getSocket(), &writefds_);
	return 0;
}

int SelectDispatcher::dispatch(int timeout)
{
	struct timeval timeoutVal;
	timeoutVal.tv_sec = timeout;
	timeoutVal.tv_usec = 0;
	fd_set readTemp = readfds_;	// 防止被select修改而丢失data数据
	fd_set writeTemp = writefds_;
	int count = select(maxNode_, &readTemp, &writeTemp, NULL, &timeoutVal);
	if (count == -1) {
		perror("select");
		exit(0);
	}

	for (int i = 0; i < maxNode_; i++) {
		if (FD_ISSET(i, &readTemp)) {
			evLoop_->active(i, (int)FDEvent::ReadEvent);
		}
		if (FD_ISSET(i, &writeTemp)) {
			evLoop_->active(i, (int)FDEvent::WriteEvent);
		}
	}
	return 0;
}
