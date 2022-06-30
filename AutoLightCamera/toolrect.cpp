#include "toolrect.h"

#include <QAction>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QtMath>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

qreal ToolRect::ptToLine(const QLineF &line, const QPointF &pt)
{
    QLineF l(line.p1(), pt);
    qreal angle = line.angleTo(l);
    qreal len   = l.length() * qSin(angle * M_PI / 180);
    return qAbs(len);
}

ToolRect::ToolRect()
{
    setAcceptHoverEvents(true);

    m_line.setLine(0, 0, 0, 0);
    m_width  = 50;
    m_height = 50;
    // createShape();
    m_transform = false;
    m_direction = 0;
    m_hover     = false;
    m_factor    = 1;
    m_first     = false;
    m_canEdit   = true;
}

ToolRect::~ToolRect()
{
    qDebug() << "Esvi rect QGraphicsItem destory!";
}

void ToolRect::reset()
{
    //准备好Item的几何形状改变。做好更新
    prepareGeometryChange();
    setCanEdit(true);

    m_line.setLine(0, 0, 0, 0);
    m_width     = 50;
    m_height    = 50;
    m_transform = false;
    m_direction = 0;
    m_hover     = false;

    // createShape();
    m_polygon.clear();
    m_polygon << QPointF(-m_height / 5, -m_height / 5) << QPointF(m_height / 5, -m_height / 5) << QPointF(m_height / 5, m_height / 5) << QPointF(-m_height / 5, m_height / 5)
              << QPointF(-m_height / 5, -m_height / 5);
    m_first = true;
}

const QPolygonF &ToolRect::getPolygon()
{
    return m_polygon;
}

void ToolRect::clearRect()
{
    m_hover = false;
    m_polygon.clear();
}

void ToolRect::setCanEdit(bool flag)
{
    m_canEdit = flag;

    if (m_canEdit)
    {
        setAcceptHoverEvents(true);
        setAcceptedMouseButtons(Qt::AllButtons);
    }
    else
    {
        setAcceptHoverEvents(false);
        setAcceptedMouseButtons(nullptr);
    }
}

void ToolRect::createShape()
{
    qreal theta;
    theta = m_line.angle();
    qDebug() << "theta" << theta;
    theta = (theta + 90) * M_PI / 180;

    qreal dx  = m_height * qCos(theta);
    qreal dy  = m_height * qSin(theta);
    QLineF l1 = m_line.translated(dx, -dy);
    QLineF l2 = m_line.translated(-dx, +dy);

    //    qDebug() << "m_line.length" << m_line.length();
    //最终的宽高
    m_height = m_height * 2;
    m_width  = m_line.length();
    //    qDebug() << "rect m_height" << m_height << "rect m_height" << m_height;

    m_polygon.clear();
    m_polygon << l1.p1() << l1.p2() << l2.p2() << l2.p1() << l1.p1();

    prepareGeometryChange();
}

QRectF ToolRect::boundingRect() const
{
    QRectF rect;
    rect = m_polygon.boundingRect();

    qreal d = 15 / m_factor + 1;
    rect.adjust(-d, -d, d, d);
    return rect;
}

void ToolRect::rectFeatures(double &row, double &col, double &phi, double &len1, double &len2)
{
    QPointF ct = pos() + m_line.center();
    row        = ct.y();
    col        = ct.x();
    len1       = m_line.length() / 2;
    len2       = qAbs(m_height); //消除可能的负值
    phi        = m_line.angle(); //角度值
    // phi        = M_PI * phi / 180;
}

QRectF ToolRect::getQRectF()
{
    QRectF r;
    if (m_polygon.size() == 5)
    {
        QPointF pt1, pt2;
        pt1.rx() = m_polygon.at(0).x() + pos().x();
        pt1.ry() = m_polygon.at(0).y() + pos().y();
        pt2.rx() = m_polygon.at(2).x() + pos().x();
        pt2.ry() = m_polygon.at(2).y() + pos().y();
        r        = QRectF(pt1, pt2).normalized();
    }
    return r;
}

// CvRect ToolRect::mapToCv()
//{
//    CvRect r;
//    if (m_polygon.size() == 5)
//    {
//        QPointF pt1, pt2;
//        pt1.rx()   = m_polygon.at(0).x() + pos().x();
//        pt1.ry()   = m_polygon.at(0).y() + pos().y();
//        pt2.rx()   = m_polygon.at(2).x() + pos().x();
//        pt2.ry()   = m_polygon.at(2).y() + pos().y();
//        QRect rect = QRectF(pt1, pt2).normalized().toRect();
//        QPoint pt  = rect.topLeft();
//        r          = CvRect(pt.x(), pt.y(), rect.width(), rect.height());
//    }
//    return r;
//}

void ToolRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QTransform &tf = painter->transform();
    m_factor             = tf.m11();

    QPen pen(Qt::red);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->drawPolygon(m_polygon);

    if (m_hover)
    {
        //拉变边控制点
        QLineF l;
        pen.setColor(Qt::gray);
        pen.setWidth(15);
        pen.setCapStyle(Qt::FlatCap); // Qt::SquareCap
        painter->setPen(pen);
        l.setPoints(m_polygon.at(0), m_polygon.at(1));
        painter->drawPoint(l.center());
        l.setPoints(m_polygon.at(2), m_polygon.at(3));
        painter->drawPoint(l.center());
        l.setPoints(m_polygon.at(1), m_polygon.at(2));
        painter->drawPoint(l.center());
        l.setPoints(m_polygon.at(3), m_polygon.at(0));
        painter->drawPoint(l.center());
    }

    if (!m_canEdit)
    {
        painter->setCompositionMode(QPainter ::CompositionMode_Difference);
        painter->setBrush(QBrush(Qt::gray, Qt::SolidPattern));
        painter->drawPolygon(m_polygon);
    }
}

void ToolRect::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    this->setCursor(Qt::SizeAllCursor);
    QGraphicsItem::hoverEnterEvent(event);
}

void ToolRect::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();

    //找到最近的边界线
    m_direction = 5;

    qreal min = 15 / m_factor / 2 + 1;
    for (int i = 0; i < m_polygon.count() - 1; i++)
    {
        qreal d = ptToLine(QLineF(m_polygon.at(i), m_polygon.at(i + 1)), pos);
        if (d < min)
        {
            min         = d;
            m_direction = i + 1;
        }
    }

    //根据位置不同设置不同的光标,
    switch (m_direction)
    {
    case 1:
    case 3: this->setCursor(Qt::SplitHCursor); break;
    case 2:
    case 4:
        this->setCursor(Qt::SplitHCursor);
        // this->setCursor(Qt::PointingHandCursor);//旋转功能取消
        break;
    case 5: this->setCursor(Qt::SizeAllCursor); break;
    default: this->setCursor(Qt::ArrowCursor); break;
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void ToolRect::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    this->setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void ToolRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "ToolRect LeftButton";
    if (event->button() == Qt::LeftButton)
    {
        m_start     = event->scenePos(); //返回鼠标光标在场景中的坐标
        m_transform = true;
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void ToolRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //    qDebug() << "ToolRect mouseMoveEvent";
    if (m_first)
    {
        //第一次建立，点击-拉动
        m_first     = false;
        m_direction = 6;
    }

    if (m_transform)
    {
        m_end = event->scenePos();
        QLineF l(m_start, m_end);
        m_start = m_end;
        //        qDebug() << "toolrect = " << m_direction;
        switch (m_direction)
        {
        case 1:
        {
            qreal alfa = l.angle() - m_line.angle();
            l.setAngle(alfa);
            m_height = (2 * m_height - l.dy()) / 2;
            // if (m_height < kPickSize)m_height = kPickSize;
            createShape();
            moveBy(0, l.dy() / 2);
            break;
        }
        case 3:
        {
            qreal alfa = l.angle() - m_line.angle();
            l.setAngle(alfa);
            m_height = (2 * m_height + l.dy()) / 2;
            // if (m_height < kPickSize)m_height = kPickSize;
            createShape();
            moveBy(0, l.dy() / 2);
            break;
        }
        case 2:
        {
            QPointF pt;
            pt.setY(m_line.p1().y());
            pt.setX(event->pos().x());
            m_line.setP2(pt);
            // m_line.setP2(event->pos());
            createShape();
            break;
        }
        case 4:
        {
            QPointF pt;
            pt.setY(m_line.p2().y());
            pt.setX(event->pos().x());
            m_line.setP1(pt);
            // m_line.setP1(event->pos());
            createShape();
            break;
        }
        case 5: moveBy(l.dx(), l.dy()); break;
        case 6:
        {
            QPointF pt = event->pos();
            qDebug() << "pt" << pt << "pt.y() / 2" << pt.y() / 2; // QPointF(1068.57,731.561) pt.y() / 2 365.78
            m_line.setP1(QPointF(0, pt.y() / 2));
            m_line.setP2(QPointF(pt.x(), pt.y() / 2));
            m_height = pt.y() / 2;
            //            m_height = pt.x() / 2;

            //
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

void ToolRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "ToolRect mouseReleaseEvent";
    if (m_transform)
    {
        m_transform = false;
        this->setCursor(Qt::ArrowCursor);

        //修改
        QMenu menu;
        QAction *okAction       = menu.addAction("确定");
        QAction *cancelAction   = menu.addAction("取消");
        QAction *selectedAction = menu.exec(event->screenPos());
        if (selectedAction == okAction) { emit detectChanged(this); }
        else
        {
            setPos(0, 0);
            setVisible(false);
        }
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

// QDataStream &operator<<(QDataStream &out, const ToolRect &item)
//{
//    out << item.pos();
//    out << item.m_line;
//    out << item.m_height;
//    return out;
//}

// QDataStream &operator>>(QDataStream &in, ToolRect &item)
//{
//    QPointF pos;
//    in >> pos;
//    in >> item.m_line;
//    in >> item.m_height;
//    item.setPos(pos);
//    item.createShape();
//    return in;
//}
