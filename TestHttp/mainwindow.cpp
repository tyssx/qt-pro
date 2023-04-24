#include "mainwindow.h"

#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    ui->lineEdit->setText(QString("http://www.baidu.com/?tn=sitehao123_15"));
    ui->lineEdit->setText(QString("http://14.215.177.38:80"));
    //关联请求完毕，会发出一个信号 finished
    connect(&manager, &QNetworkAccessManager::finished, this, &MainWindow::read_data);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonSend_clicked()
{
    // ui->lineEdit->text();//获取到单行输入框的内容
    // QUrl 将输入的字符串解析成http地址

    QUrl url(ui->lineEdit->text());
    //创建一个请求对象
    QNetworkRequest request(url);
    //发送请求
    manager.get(request);
    //    http://timgsa.baidu.com/timg?image&quality
    //    http://www.baidu.com/?tn=sitehao123_15
}

void MainWindow::read_data(QNetworkReply *app)
{
    QByteArray array = app->readAll();
    qDebug() << "error" << app->error();
    qDebug() << "size" << array.size();
    QByteArray a = array.split('\n').first();
    QString str  = QString(a);
    //    QString str  = QString(array);
    qDebug() << "array" << str;

    //创建图片空间
    //    QPixmap mmp;
    //    mmp.loadFromData(array);

    //    //设置图片显示的大小，让图片符合label显示框的大小
    //    mmp = mmp.scaled(ui->label->size());

    //    //在label中显示图片
    //    ui->label->setPixmap(mmp);
}

void MainWindow::on_pushButtonGetInfo_clicked()
{
    QString imei       = "358638087646241";
    QString station_no = "PPACK008";
    getWeighInfoRequest(imei, station_no);
}

int MainWindow::getWeighInfoRequest(QString imei, QString station_no)
{
    // IMEI=358638087646241&&Station_No=PPACK008
    QNetworkRequest request;
    QString scheme        = "http";
    QString serverAddr    = "10.250.112.112";
    QString port          = "6900";
    QString requestHeader = scheme + tr("://") + serverAddr + tr(":") + port;

#if 1
    QString fullRequest = requestHeader +
        tr("/mes/pack/Get_Wt_Info?IMEI=%1&&Station_No=%2").arg(imei).arg(station_no);

    request.setUrl(QUrl(fullRequest));

    //发送Get请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);

#else
    QString fullRequest = requestHeader + tr("/mes/pack/Get_Wt_Info?").arg(imei).arg(station_no);

    request.setUrl(QUrl(fullRequest));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    //获取对应的参数数据
    QByteArray data;
    data.append("IMEI=");
    data.append(imei.toUtf8());
    data.append("&&Station_No=");
    data.append(station_no.toUtf8());

    //发送Post请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, data);

#endif
    QEventLoop eventLoop;
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    //请求返回异常进行处理
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "reply error" << reply->error();
        return reply->error();
    }

    //解析返回的Json结果
#if 0
    //    QByteArray replyData = reply->readAll();
#else
    QString path         = QApplication::applicationDirPath();
    qDebug() << "path" << path;
    QFile file(path + tr("/weigh.json"));
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "file open failed";
        return -1;
    }
    QByteArray replyData = file.readAll();

#endif
    QJsonParseError jsonErr;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &jsonErr));
    if (jsonErr.error != QJsonParseError::NoError)
    {
        qDebug() << "get json error" << jsonErr.error;
        return -1;
    }
    QJsonObject rootObj = jsonDoc.object();
    QString woNoStr     = rootObj.value("WO_NO").toString();
    qDebug() << "woNoStr" << woNoStr;
}
