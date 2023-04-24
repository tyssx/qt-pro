#include "netthread.h"

#include <QDateTime>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <QWaitCondition>

NetThread::NetThread(
    QWaitCondition *condition, QMutex *mutex, QQueue<QString> *queue, QObject *parent)
    : QObject(parent)
{
    m_condition = condition;
    m_mutex     = mutex;
    m_queue     = queue;

    //这里用定时器模拟网络发送数据
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, this, &NetThread::slot_addQueue);
    m_timer->start(20);
}

void NetThread::slot_addQueue()
{
    m_mutex->lock();
    // void *threadId = QThread::currentThreadId();
    QString time = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm::ss ");
    m_queue->enqueue(time + m_threadId + " send data");
    m_condition->wakeAll();

    m_mutex->unlock();
}
