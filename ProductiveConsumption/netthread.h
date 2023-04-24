#ifndef NETTHREAD_H
#define NETTHREAD_H

#include <QObject>

class QWaitCondition;
class QMutex;
class QTimer;

class NetThread : public QObject
{
    Q_OBJECT
public:
    explicit NetThread(QWaitCondition *condition,
        QMutex *mutex,
        QQueue<QString> *queue,
        QObject *parent = nullptr);

    void setThreadId(QString threadId) { m_threadId = threadId; }

signals:

public slots:
    void slot_addQueue();

private:
    QWaitCondition *m_condition;
    QMutex *m_mutex;
    QQueue<QString> *m_queue;
    QTimer *m_timer;
    QString m_threadId;
};

#endif // NETTHREAD_H
