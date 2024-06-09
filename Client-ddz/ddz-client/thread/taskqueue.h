#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include "cards.h"
#include <QObject>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

struct Task {
    int bet = 0;
    Cards cs;
};

class TaskQueue : public QObject
{
    Q_OBJECT
public:
    TaskQueue(const TaskQueue& t) = delete;
    TaskQueue& operator=(const TaskQueue& t) = delete;

    static TaskQueue* getInstance();

    // 添加任务
    void add(Task &t);
    // 取出任务
    Task take();
    // 获取队列中任务数量
    int size();
    // 清空队列
    void clear();

private:
    explicit TaskQueue(QObject *parent = nullptr);

private:
    static TaskQueue taskQ_;
    QQueue<Task> queue_;
    QMutex mutex_;
    QWaitCondition condition_;
};

#endif // TASKQUEUE_H
