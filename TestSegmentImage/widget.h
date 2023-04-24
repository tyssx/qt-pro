#ifndef WIDGET_H
#define WIDGET_H

#include <QGraphicsPixmapItem>
#include <QWidget>
#include <vxlib.h>

class GraphicsScene;
class ExtVariation3ChItem;
class ExtPolygonitem;

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

    void slot_addItem(ExtVariation3ChItem *item);
    void slot_apply();
    void slot_esviItem_update(QGraphicsItem *cur);

public slots:
    void slot_getEdges(CxPointArray roiArray, CxPointArray segment, CxPointArray CenterArea);

private:
    Ui::Widget *ui;
    GraphicsScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem; //图像
    QImage m_image;
    INT64 handle;
    ExtVariation3ChItem *m_item;
    ExtPolygonitem *m_Polygonitem;
};

#endif // WIDGET_H
