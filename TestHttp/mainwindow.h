#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>

class QNetworkReply;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonGetInfo_clicked();
    //读取信息
    void read_data(QNetworkReply *app);

private:
    int getWeighInfoRequest(QString imei, QString station_no); //获取称重过站信息请求

private:
    Ui::MainWindow *ui;

    //声明一个管理对象
    QNetworkAccessManager manager;
};

#endif // MAINWINDOW_H
