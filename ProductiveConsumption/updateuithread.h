#ifndef UPDATEUITHREAD_H
#define UPDATEUITHREAD_H

#include <QObject>

class QWaitCondition;
class QMutex;
class QTimer;

class UpdateUiThread : public QObject
{
    Q_OBJECT
public:
    explicit UpdateUiThread(QWaitCondition *condition,
        QMutex *mutex,
        QQueue<QString> *queue,
        QObject *parent = nullptr);

signals:
    void updateUi(QString netData);

public slots:
    void slot_updateUi();

private:
    QWaitCondition *m_condition;
    QMutex *m_mutex;
    QQueue<QString> *m_queue;
    QTimer *m_timer;
};

#endif // UPDATEUITHREAD_H
