#include "widget.h"

#include "netthread.h"
#include "ui_widget.h"
#include "updateuithread.h"

#include <QThread>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_queue.clear();
    m_netThread1   = new QThread;
    m_netThread2   = new QThread;
    m_updateThread = new QThread;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    UpdateUiThread *updateObject = new UpdateUiThread(&m_condition, &m_mutex, &m_queue);
    updateObject->moveToThread(m_updateThread);

    connect(this, &Widget::startUpdateUi, updateObject, &UpdateUiThread::slot_updateUi);
    connect(updateObject, &UpdateUiThread::updateUi, this, [=](QString netData) {
        ui->plainTextEdit->appendPlainText(netData);
    });
    ui->plainTextEdit->appendPlainText("启动刷新UI线程");
    m_updateThread->start();
    emit startUpdateUi();
}

void Widget::on_pushButton_2_clicked()
{
    NetThread *netObject = new NetThread(&m_condition, &m_mutex, &m_queue);
    netObject->moveToThread(m_netThread1);
    netObject->setThreadId("网络线程1");
    ui->plainTextEdit->appendPlainText("启动网络线程1");
    m_netThread1->start();
}

void Widget::on_pushButton_3_clicked()
{
    NetThread *netObject = new NetThread(&m_condition, &m_mutex, &m_queue);
    netObject->moveToThread(m_netThread2);
    netObject->setThreadId("网络线程2");
    ui->plainTextEdit->appendPlainText("启动网络线程2");
    m_netThread2->start();
}
