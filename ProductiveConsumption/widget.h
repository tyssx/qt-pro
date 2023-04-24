#ifndef WIDGET_H
#define WIDGET_H

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QWidget>

class NetThread;
class UpdateUiThread;

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Widget *ui;

    QWaitCondition m_condition;
    QMutex m_mutex;
    QQueue<QString> m_queue;

    QThread *m_netThread1;
    QThread *m_netThread2;
    QThread *m_updateThread;

signals:
    void startUpdateUi();
};

#endif // WIDGET_H
