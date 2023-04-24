#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

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
    void on_pushButtonSend_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *m_tcpSocket;
};

#endif // WIDGET_H
