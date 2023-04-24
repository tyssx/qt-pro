#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QTcpServer;
class QTcpSocket;

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
    void slot_newConnect();

    void on_pushButtonSend_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *m_tcpServer;
    QTcpSocket *m_tcpSocket;
};

#endif // WIDGET_H
