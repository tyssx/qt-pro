#include "graphicsview.h"

#include <QApplication>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QtDebug>

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_drag = false;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && DragMode() == QGraphicsView::NoDrag)
    {
        qDebug() << "Left";
        QPoint point        = event->pos();
        QGraphicsItem *item = itemAt(point);
        bool click          = false;
        if (!item) { click = true; }
        else
        {
            if (item->type() < QGraphicsItem::UserType) { click = true; }
        }
        if (click)
        {
            qDebug() << "Left click";
            emit mouseClicked(point);
        }
    }
    else if (event->button() == Qt::MidButton) //中间键
    {
        qDebug() << "Mid";
        m_drag = true;
        //设置应用程序强制光标为 cursor
        QApplication::setOverrideCursor(QCursor(Qt::OpenHandCursor)); //手势光标
        m_start = event->pos();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag)
    {
        QPoint pt    = event->pos();
        QPoint delta = pt - m_start;
        //将当前视图位置转换到坐标(delta.x(), -delta.y())
        translate(delta.x(), -delta.y());
        m_start = pt;
    }
    QGraphicsView::mouseMoveEvent(event);
}
void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton && m_drag)
    {
        m_drag = false;
        QApplication::restoreOverrideCursor(); //设置原来的光标
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps   = numDegrees / 15.0;
    double factor     = std::pow(1.125, numSteps);
    scale(factor, factor);
}
