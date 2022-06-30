#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

//输出debug日志
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    switch (type)
    {
    case QtDebugMsg: strMsg = QString("Debug:"); break;
    case QtWarningMsg: strMsg = QString("Warning:"); break;
    case QtCriticalMsg: strMsg = QString("Critical:"); break;
    case QtFatalMsg: strMsg = QString("Fatal:"); break;
    }

    // 设置输出信息格式
    //    QString context_info      = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    //    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    //    QString current_date      = QString("(%1)").arg(current_date_time);
    //    QString strMessage           = QString("%1 %2 %3 %4").arg(strMsg).arg(context_info).arg(msg).arg(current_date);
    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString strMessage  = QString("%1 File:%2  Line:%3  Function:%4 (---Message:%5---)").arg(strDateTime).arg(context.file).arg(context.line).arg(context.function).arg(localMsg.constData());
    /*DateTime:*/
    // 输出信息至文件中（读写、追加形式）
    QString exePath = QApplication::applicationDirPath();
    QFile file(exePath + "/log/log.txt");
    // QFile file("log.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();

    // 解锁
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    std::cout << "start main" << endl;
    QApplication a(argc, argv);
    // 安装消息处理程序
    qInstallMessageHandler(myMessageOutput);

    MainWindow w;
    w.show();
    // 打印信息
    qDebug("This is a debug message.");
    //    qWarning("This is a warning message.");
    //    qCritical("This is a critical message.");
    //    qFatal("This is a fatal message.");
    return a.exec();
}
