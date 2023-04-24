#include "graphicspixmapitem.h"

#include <QPainter>

GraphicsPixmapItem::GraphicsPixmapItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
}

QRectF GraphicsPixmapItem::boundingRect() const
{
    return m_image.rect();
}

void GraphicsPixmapItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->translate(-0.5, -0.5);
    // QGraphicsPixmapItem::paint(painter, option, widget);
    painter->drawImage(0, 0, m_image);
    painter->translate(0.5, 0.5);
    painter->drawRect(boundingRect());

    //显示模板框
    if (m_templateShow)
    {
        QPen pen(Qt::red);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawPath(m_path);
        painter->fillPath(m_path, QBrush(QColor(127, 127, 127, m_templateAlpha)));
    }
}

void GraphicsPixmapItem::setTemplateDisplay(bool show, int alpha)
{
    m_templateShow  = show;
    m_templateAlpha = alpha;
    update();
}

void GraphicsPixmapItem::setPixmap(const QImage &img)
{
    m_image = img;
    update();
}
