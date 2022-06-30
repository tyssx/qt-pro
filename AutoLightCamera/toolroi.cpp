#include "toolroi.h"

#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

//此函数在esviitem.h中
qreal ptToLine(const QLineF &line, const QPointF &pt)
{
    QLineF l(line.p1(), pt);
    qreal angle = line.angleTo(l);
    qreal len   = l.length() * qSin(angle * M_PI / 180);
    return qAbs(len);
}

ToolRoi::ToolRoi()
{
    //使图形项可以接收悬停事件
    setAcceptHoverEvents(true);

    m_line.setLine(0, 0, 0, 0);
    m_width = 50;
    //创建一个形状
    createShape();

    m_transform = false;
    m_direction = 0;
    m_hover     = false;
    m_factor    = 1;
    m_first     = false;
}

ToolRoi::~ToolRoi()
{
    qDebug() << "ToolRoi destory!";
}

void ToolRoi::reset()
{
    prepareGeometryChange();

    m_line.setLine(0, 0, 0, 0);

    m_width     = 50;
    m_transform = false;
    m_direction = 0;
    m_hover     = false;

    // createShape();
    m_polygon.clear();
    m_polygon << QPointF(-m_width / 5, -m_width / 5) << QPointF(m_width / 5, -m_width / 5) << QPointF(m_width / 5, m_width / 5) << QPointF(-m_width / 5, m_width / 5)
              << QPointF(-m_width / 5, -m_width / 5);
    m_first = true;
}

/*
void ToolRoi::editorDetect() {

    emit detectChanged(this);

}
*/

void ToolRoi::createShape()
{
    qreal theta;
    theta = m_line.angle();
    theta = (theta + 90) * M_PI / 180;

    qreal dx  = m_width * qCos(theta);
    qreal dy  = m_width * qSin(theta);
    QLineF l1 = m_line.translated(dx, -dy);
    QLineF l2 = m_line.translated(-dx, +dy);

    m_polygon.clear();
    m_polygon << l1.p1() << l1.p2() << l2.p2() << l2.p1() << l1.p1();

    prepareGeometryChange();
}

void ToolRoi::setRect(QRect rect)
{
    static QRect org;

    if (rect.isNull())
        rect = org;
    else
        org = rect;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    m_line  = QLineF(x1, (y1 + y2) / 2, x2, (y1 + y2) / 2);
    m_width = rect.height() / 2;
    setPos(0, 0);
    createShape();

    emit toolChange();
}

QRectF ToolRoi::boundingRect() const
{
    QRectF rect;
    rect = m_polygon.boundingRect();

    qreal d = kPickSize / m_factor; //+1
    rect.adjust(-d, -d, d, d);
    return rect;
}

QPainterPath ToolRoi::shape() const
{
    QPainterPath path;
    path.addPolygon(m_polygon);
    return path;
}

void ToolRoi::rectFeatures(double &row, double &col, double &phi, double &len1, double &len2)
{
    QPointF ct = pos() + m_line.center();
    row        = ct.y();
    col        = ct.x();
    len1       = m_line.length() / 2;
    len2       = m_width;
    phi        = m_line.angle();
    phi        = M_PI * phi / 180;
}

QPolygonF ToolRoi::getPolygonF()
{
    return m_polygon;
}

void ToolRoi::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QTransform &tf = painter->transform();
    m_factor             = tf.m11();

    QPen pen(Qt::green);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPolygon(m_polygon);

    if (m_hover)
    {
        //拉变边控制点
        QLineF l;
        pen.setColor(Qt::gray);
        pen.setWidth(kPickSize);
        pen.setCapStyle(Qt::FlatCap); // Qt::SquareCap
        painter->setPen(pen);
        l.setPoints(m_polygon.at(0), m_polygon.at(1));
        painter->drawPoint(l.center());
        l.setPoints(m_polygon.at(2), m_polygon.at(3));
        painter->drawPoint(l.center());

        //旋转边控制点
        pen.setCapStyle(Qt::RoundCap);
        painter->setPen(pen);
        l.setPoints(m_polygon.at(1), m_polygon.at(2));
        painter->drawPoint(l.center());
        l.setPoints(m_polygon.at(3), m_polygon.at(0));
        painter->drawPoint(l.center());
    }

    /*
    //被选中
    if (option->state & QStyle::State_Selected && !m_transform) {
      QPen pen1(Qt::DashLine);
      pen1.setCosmetic(true);
      pen1.setColor(Qt::gray);
      painter->setPen(pen1);
      painter->drawRect(boundingRect());
    }
    */
}

void ToolRoi::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    this->setCursor(Qt::SizeAllCursor);
    QGraphicsItem::hoverEnterEvent(event);
}

void ToolRoi::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();

    //找到最近的边界线
    m_direction = 5;

    qreal min = kPickSize / m_factor / 2 + 1;
    for (int i = 0; i < m_polygon.count() - 1; i++)
    {
        qreal d = ptToLine(QLineF(m_polygon.at(i), m_polygon.at(i + 1)), pos);
        if (d < min)
        {
            min         = d;
            m_direction = i + 1;
        }
    }

    //根据位置不同可设置不同的光标
    switch (m_direction)
    {
    case 1:
    case 3: this->setCursor(Qt::SplitHCursor); break;
    case 2:
    case 4: this->setCursor(Qt::PointingHandCursor); break;
    case 5: this->setCursor(Qt::SizeAllCursor); break;
    default: this->setCursor(Qt::ArrowCursor); break;
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void ToolRoi::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    this->setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void ToolRoi::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_start     = event->scenePos();
        m_transform = true;
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void ToolRoi::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_transform)
    {
        if (m_first)
        {
            //第一次建立，点击-拉动
            m_first     = false;
            m_direction = 6;
        }

        m_end = event->scenePos();
        QLineF l(m_start, m_end);
        m_start = m_end;
        switch (m_direction)
        {
        case 1:
        {
            qreal alfa = l.angle() - m_line.angle();
            l.setAngle(alfa);
            m_width -= l.dy();
            // if (m_width < kPickSize)m_width = kPickSize;
            createShape();
            break;
        }
        case 3:
        {
            qreal alfa = l.angle() - m_line.angle();
            l.setAngle(alfa);
            m_width += l.dy();
            // if (m_width < kPickSize)m_width = kPickSize;
            createShape();
            break;
        }
        case 2:
            m_line.setP2(event->pos());
            createShape();
            break;
        case 4:
            m_line.setP1(event->pos());
            createShape();
            break;
        case 5: moveBy(l.dx(), l.dy()); break;
        case 6:
        {
            QPointF pt = event->pos();
            m_line.setP1(QPointF(0, pt.y() / 2));
            m_line.setP2(QPointF(pt.x(), pt.y() / 2));
            m_width = pt.y() / 2;
            createShape();

            break;
        }
        default: break;
        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ToolRoi::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_transform)
    {
        m_transform = false;
        emit toolChange();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void ToolRoi::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setRect();
}
