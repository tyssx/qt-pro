#ifndef EXTPOLYGONITEM_H
#define EXTPOLYGONITEM_H

#include <QGraphicsObject>

class ExtPolygonitem : public QGraphicsObject
{
public:
    ExtPolygonitem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setColor(const QColor &color) { brushColor = color; }

private:
    QColor brushColor;
};

#endif // EXTPOLYGONITEM_H
