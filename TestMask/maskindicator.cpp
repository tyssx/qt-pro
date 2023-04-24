#include "maskindicator.h"

#include <QPainter>

const int PickSize = 15; //拾取精度/控制点大小

MaskIndicator::MaskIndicator(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    m_isFinish = false;
    setAcceptHoverEvents(false);
    hide();
}

MaskIndicator::~MaskIndicator()
{
}

QRectF MaskIndicator::boundingRect() const
{
    QRectF rect;
    rect = m_polygon.boundingRect();

    qreal d = (PickSize + 1) / m_factor / 2;
    rect.adjust(-d, -d, d, d);
    return rect;
}

QPainterPath MaskIndicator::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygon);

    qreal d = (PickSize + 1) / m_factor;
    QPainterPathStroker ps;
    ps.setWidth(d);
    path = path.united(ps.createStroke(path));

    return path;
}

void MaskIndicator::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QTransform &tf = painter->transform();
    m_factor             = tf.m11();
}
