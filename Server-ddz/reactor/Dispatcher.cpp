#include "Dispatcher.h"

Dispatcher::Dispatcher(EventLoop* evLoop)
	: evLoop_(evLoop)
{}

Dispatcher::~Dispatcher()
{
}

int Dispatcher::add()
{
	return 0;
}

int Dispatcher::remove()
{
	return 0;
}

int Dispatcher::modify()
{
	return 0;
}

int Dispatcher::dispatch(int timeout)
{
	return 0;
}
