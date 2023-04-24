#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QWidget>
#include <vxlib.h>

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
    void on_pushButtonLoad_clicked();

    void on_pushButtonToINT64_clicked();

    void on_pushButtonClear_clicked();

private:
    Ui::Widget *ui;
    QGraphicsScene *m_scene1, *m_scene2;
    QGraphicsPixmapItem *m_pixmapItem1, *m_pixmapItem2; //图像
    QImage m_image;
};

#endif // WIDGET_H
