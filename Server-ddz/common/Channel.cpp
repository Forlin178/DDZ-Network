#include "Channel.h"
#include <stdio.h>
#include <stdlib.h>

Channel::Channel(int fd, FDEvent events, handleFunc readCallBack, handleFunc writeCallBack, handleFunc destroyCallBack, void* arg)
{
	this->fd_ = fd;
	this->events_ = (int)events;
	this->readCallBack = readCallBack;
	this->writeCallBack = writeCallBack;
	this->destroyCallBack = destroyCallBack;
	this->arg_ = arg;
}

void Channel::writeEventEnable(bool isEnable)
{
	if (isEnable) {
		this->events_ |= (int)FDEvent::WriteEvent;
	}
	else {
		this->events_ = this->events_ & ~(int)FDEvent::WriteEvent;
	}
}

bool Channel::isWriteEventEnable()
{
	return this->events_ & (int)FDEvent::WriteEvent;	// 如果enable则返回大于0
}

void Channel::setCurrentEvent(FDEvent event) {
    events_ = static_cast<int>(event);
}

bool Channel::isReadEventEnable() {
    return this->events_ & static_cast<int>(FDEvent::ReadEvent);
}
