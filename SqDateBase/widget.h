#ifndef WIDGET_H
#define WIDGET_H

#include <QSqlDatabase>
#include <QWidget>

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

private:
    void connectBase();
    void createTable();

private:
    Ui::Widget *ui;
    QSqlDatabase db;
};

#endif // WIDGET_H
