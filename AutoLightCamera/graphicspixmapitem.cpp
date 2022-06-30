#include "graphicspixmapitem.h"

#include <QPainter>

GraphicsPixmapItem::GraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsPixmapItem(parent)
{
}

void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->translate(-0.5, -0.5);
    QGraphicsPixmapItem::paint(painter, option, widget);
    painter->translate(0.5, 0.5);
    painter->drawRect(boundingRect());

    if (!m_templateRect.isNull())
    {
        QPen pen(Qt::blue);
        pen.setWidth(10);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        QRectF rectf(m_templateRect.left() - 50, m_templateRect.top() - 50, m_templateRect.width() + 100, m_templateRect.height() + 100);
        painter->drawRect(rectf);
        painter->fillRect(rectf, QBrush(QColor(127, 127, 127, 20)));
    }
}
