#include "taskqueue.h"

TaskQueue TaskQueue::taskQ_;

TaskQueue::TaskQueue(QObject *parent)
    : QObject{parent}
{}

TaskQueue *TaskQueue::getInstance()
{
    return &taskQ_;
}

void TaskQueue::add(Task &t)
{
    mutex_.lock();
    queue_.enqueue(t);
    mutex_.unlock();
    condition_.wakeAll();
}

Task TaskQueue::take()
{
    Task t;
    mutex_.lock();
    while (1) {
        if (queue_.size() > 0){
            t = queue_.dequeue();
            break;
        }
        else
            condition_.wait(&mutex_);
    }
    mutex_.unlock();
    return t;
}

int TaskQueue::size()
{
    mutex_.lock();
    int size = queue_.size();
    mutex_.unlock();
    return size;
}

void TaskQueue::clear()
{
    mutex_.lock();
    queue_.clear();
    mutex_.unlock();
}
