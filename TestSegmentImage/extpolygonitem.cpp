#include "extpolygonitem.h"

#include <QCursor>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QPainter>

ExtPolygonitem::ExtPolygonitem()
{
    brushColor = Qt::red;

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}

QRectF ExtPolygonitem::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-10 - adjust, -10 - adjust, 20 + adjust, 20 + adjust);
}

void ExtPolygonitem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (hasFocus()) { painter->setPen(QPen(QColor(255, 255, 255, 200))); }
    else
    {
        painter->setPen(QPen(QColor(100, 100, 100, 100)));
    }
    painter->setBrush(brushColor);
    // painter->drawRect(-10, -10, 20, 20);

    // 定义四个点
    static const QPointF points[4] = {
        QPointF(200.0, 80.0), QPointF(220.0, 10.0), QPointF(270.0, 30.0), QPointF(310.0, 70.0)};
    // 使用四个点绘制多边形
    painter->drawPolygon(points, 4);
}
