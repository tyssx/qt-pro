#include "widget.h"

#include "ui_widget.h"

#include <QDateTime>
#include <QTcpSocket>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->connectToHost("127.0.0.1", 8888);
    connect(m_tcpSocket, &QTcpSocket::connected, [=]() {
        QString str  = "成功与[%1:%2]建立连接...";
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->plainTextEdit->appendPlainText(str + time);
        connect(m_tcpSocket, &QTcpSocket::readyRead, this, [=]() {
            ui->plainTextEdit->appendPlainText(m_tcpSocket->readAll());
        });
    });
    connect(m_tcpSocket, &QTcpSocket::disconnected, [=]() {
        ui->plainTextEdit->appendPlainText("服务端连接已断开");
    });
}

Widget::~Widget()
{
    m_tcpSocket->disconnectFromHost();
    m_tcpSocket->close();
    delete ui;
}

void Widget::on_pushButtonSend_clicked()
{
    QString data = ui->lineEdit->text();
    m_tcpSocket->write(data.toUtf8().data());
}
