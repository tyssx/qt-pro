#include "arrowsbutton.h"

#include <QDebug>
#include <QEvent>
#include <QPainter>

ArrowsButton::ArrowsButton(/*int dir, QSize size, */ QWidget *parent)
    : QPushButton(parent) /*, m_direction(dir)*/
{
    setFocusPolicy(Qt::NoFocus);
    //    setFixedSize(size);
    installEventFilter(this);
}

void ArrowsButton::init(int dir /*, QSize size*/)
{
    m_direction = dir;
    //    setFixedSize(size);
}

void ArrowsButton::resizeEvent(QResizeEvent *event)
{
    int w = width();
    int h = height();
    qDebug() << "w h" << w << h;

    QPointF pt[7]; //绘制箭头的点

    if (m_direction == left)
    {
        pt[0] = QPointF(0, h / 2);
        pt[1] = QPointF(h / 2, h);
        pt[2] = QPointF(h / 2, 3 * h / 4);
        pt[3] = QPointF(w, 3 * h / 4);
        pt[4] = QPointF(w, h / 4);
        pt[5] = QPointF(h / 2, h / 4);
        pt[6] = QPointF(h / 2, 0);
    }
    else if (m_direction == right)
    {
        pt[0] = QPointF(w, h / 2);
        pt[1] = QPointF(w - h / 2, h);
        pt[2] = QPointF(w - h / 2, 3 * h / 4);
        pt[3] = QPointF(0, 3 * h / 4);
        pt[4] = QPointF(0, h / 4);
        pt[5] = QPointF(w - h / 2, h / 4);
        pt[6] = QPointF(w - h / 2, 0);
    }
    else if (m_direction == front)
    {
        pt[0] = QPointF(5 * w / 6, 0);
        pt[1] = QPointF(w / 3, h / 3);
        pt[2] = QPointF(w / 2, h / 3);
        pt[3] = QPointF(0, h);
        pt[4] = QPointF(w / 3, h);
        pt[5] = QPointF(5 * w / 6, h / 3);
        pt[6] = QPointF(w, h / 3);
    }
    else if (m_direction == back)
    {
        pt[0] = QPointF(w / 6, h);
        pt[1] = QPointF(0, 2 * h / 3);
        pt[2] = QPointF(w / 6, 2 * h / 3);
        pt[3] = QPointF(2 * w / 3, 0);
        pt[4] = QPointF(w, 0);
        pt[5] = QPointF(w / 2, 2 * h / 3);
        pt[6] = QPointF(2 * w / 3, 2 * h / 3);
    }
    QPolygonF polygon; //多边形
    for (int i = 0; i < 7; i++) { polygon << pt[i]; }
    m_path = QPainterPath();
    m_path.addPolygon(polygon);
}

void ArrowsButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(0, 0, 0));
    QBrush brush(QColor(0, 180, 255));
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawPath(m_path);
    if (m_hover)
    {
        brush.setColor(QColor(0, 160, 255));
        painter.setBrush(brush);
        painter.drawPath(m_path);
    }
    if (m_press)
    {
        brush.setColor(QColor(0, 120, 255));
        painter.setBrush(brush);
        painter.drawPath(m_path);
    }
}

bool ArrowsButton::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::HoverEnter)
    {
        m_hover = true;
        update();
    }
    if (event->type() == QEvent::HoverLeave)
    {
        m_hover = false;
        update();
    }
    if (event->type() == QEvent::MouseButtonPress)
    {
        m_press = true;
        update();
    }
    if (event->type() == QEvent::MouseButtonRelease)
    {
        m_press = false;
        update();
    }
    return QPushButton::eventFilter(watched, event);
}

// QSize ArrowsButton::minimumSizeHint() const
//{
//    return {60, 20};
//}
