#include "widget.h"

#include "ui_widget.h"

#include <QDateTime>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_tcpServer = new QTcpServer(this);
    m_tcpServer->listen(QHostAddress::Any, 8888); //开启监听

    connect(m_tcpServer, &QTcpServer::newConnection, this, &Widget::slot_newConnect);
}

Widget::~Widget()
{
    m_tcpSocket->disconnectFromHost();
    m_tcpSocket->close();
    delete m_tcpSocket;
    delete ui;
}

void Widget::slot_newConnect()
{
    m_tcpSocket  = m_tcpServer->nextPendingConnection();
    QString ip   = m_tcpSocket->peerAddress().toString(); //获取对方IP地址
    quint16 port = m_tcpSocket->peerPort();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"); //获取当前时间
    qDebug() << "连接进来，IP" << ip << "port" << port << "time" << time;
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, [=]() {
        QString message = m_tcpSocket->readAll();
        ui->plainTextEdit->appendPlainText(message);
        qDebug() << "recv" << message;
    });
    connect(m_tcpSocket, &QTcpSocket::disconnected, [=]() {
        ui->plainTextEdit->appendPlainText("客户端连接已断开");
    });
}

void Widget::on_pushButtonSend_clicked()
{
    QString data = ui->lineEdit->text();
    m_tcpSocket->write(data.toUtf8().data());
}
