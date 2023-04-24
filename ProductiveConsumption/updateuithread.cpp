#include "updateuithread.h"

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QTimer>
#include <QWaitCondition>

UpdateUiThread::UpdateUiThread(
    QWaitCondition *condition, QMutex *mutex, QQueue<QString> *queue, QObject *parent)
    : QObject(parent)
{
    m_condition = condition;
    m_mutex     = mutex;
    m_queue     = queue;
}

void UpdateUiThread::slot_updateUi()
{
    while (true)
    {
        QString netData;
        if (m_queue->count() == 0)
        {
            m_mutex->lock();
            //生产消费者模式，主要是理解wait的机制，wait时，会阻塞在这里，且同时也会把锁打开，让生产者（NetThread）可以向
            // queue中插入数据。生产者向queue中插入数据后，需要调用condition.wakeall()或者wakeone()，当condition
            //收到wake信号时，会把锁重新锁上（暂时不让生产者向queue插入数据），并开始执行此处后面的代码
            m_condition->wait(m_mutex);
            while (m_queue->count() > 0)
            {
                //实际上进行一系列比较耗时的UI更新操作，这里用发送信号代替
                netData = m_queue->dequeue();
                emit updateUi(netData);
            }
            m_mutex->unlock();
        }
    }
}
