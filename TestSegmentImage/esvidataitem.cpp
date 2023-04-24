#include "esvidataitem.h"

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPolygon>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

QPainterPath toQPainterPath(const std::vector<Stroke> &ctr)
{
    QPainterPath path;
    QPointF pt;
    for (std::vector<Stroke>::size_type i = 0; i < ctr.size(); ++i)
    {
        const Stroke &srk = ctr.at(i);
        bool first        = true;
        for (std::vector<Pointf>::size_type j = 0; j < srk.size(); j++)
        {
            pt.rx() = static_cast<qreal>(srk.at(j).x);
            pt.ry() = static_cast<qreal>(srk.at(j).y);
            if (first)
            {
                path.moveTo(pt);
                first = false;
            }
            else
            {
                path.lineTo(pt);
            }
        }
    }
    return path;
}

std::vector<QPainterPath> toQPainterPathVector(const std::vector<Stroke> &ctr)
{
    std::vector<QPainterPath> vctPath;
    QPointF pt;
    qDebug() << "toQPainterPathVector ctr.size()" << ctr.size();
    for (std::vector<Stroke>::size_type i = 0; i < ctr.size(); ++i)
    {
        const Stroke &srk = ctr.at(i);
        bool first        = true;
        QPainterPath path;
        for (std::vector<Pointf>::size_type j = 0; j < srk.size(); j++)
        {
            pt.rx() = static_cast<qreal>(srk.at(j).x);
            pt.ry() = static_cast<qreal>(srk.at(j).y);
            if (first)
            {
                path.moveTo(pt);
                first = false;
            }
            else
            {
                path.lineTo(pt);
            }
        }
        vctPath.push_back(path);
    }
    return vctPath;
    //    bool JImage::isPointInPolygon(QVector<QPointF> points, QPointF point)
    //    {
    //        QPolygonF polygon(points);
    //        return polygon.containsPoint(point, Qt::OddEvenFill);
    //    }
}

EsviDataItem::EsviDataItem(QObject *parent) : QObject(parent)
{
    m_color    = Qt::green;
    m_penStyle = Qt::SolidLine;
}

QRectF EsviDataItem::boundingRect() const
{
    if (m_boundingRect.isNull()) { m_boundingRect |= m_extEdge.boundingRect(); }
    return m_boundingRect;
}

void EsviDataItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //选中或掠过状态背景绘制
    //    if (option->state & QStyle::State_MouseOver || option->state & QStyle::State_Selected)
    //    {
    //        QPainterPath path = strokePath();
    //        painter->fillPath(path, QBrush(QColor(255, 255, 0)));
    //    }
    //    qDebug() << "EsviDataItem paint";
    QPen pen(m_color);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setCosmetic(true);
    pen.setColor(Qt::green);
    painter->setPen(pen);
    painter->drawPath(m_extEdge);

    //三通道缺陷检测，画轮廓
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setCosmetic(true);
    for (int i = 0; i < m_extSegment.size(); i++)
    {
        switch (i)
        {
        case 0: pen.setColor(Qt::red); break;
        case 1: pen.setColor(Qt::green); break;
        case 2: pen.setColor(Qt::blue); break;
        case 3: pen.setColor(Qt::cyan); break;
        case 4: pen.setColor(Qt::magenta); break;
        case 5: pen.setColor(Qt::yellow); break;
        case 6: pen.setColor(Qt::gray); break;
        case 7: pen.setColor(Qt::darkRed); break;
        case 8: pen.setColor(Qt::darkGreen); break;
        case 9: pen.setColor(Qt::darkBlue); break;
        case 10: pen.setColor(Qt::darkCyan); break;
        case 11: pen.setColor(Qt::darkMagenta); break;
        case 12: pen.setColor(Qt::darkYellow); break;
        case 13: pen.setColor(Qt::darkGray); break;
        case 14: pen.setColor(Qt::lightGray); break;
        default: pen.setColor(Qt::green); break;
        }
        painter->setPen(pen);
        painter->drawPath(m_extSegment.at(i));
        painter->drawText(
            m_extSegment.at(i).boundingRect(), Qt::AlignCenter, QString::asprintf("区域%d", i));
    }
}

void EsviDataItem::setExtEdge(const std::vector<Stroke> &ctr)
{
    m_extEdge      = toQPainterPath(ctr);
    m_boundingRect = QRectF();
    prepareGeometryChange();
}

void EsviDataItem::setExtSegment(const std::vector<Stroke> &ctr)
{
    m_extSegment   = toQPainterPathVector(ctr);
    m_boundingRect = QRectF();
    prepareGeometryChange();
}
