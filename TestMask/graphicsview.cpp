#include "graphicsview.h"

#include "customiconstyle.h"
#include "graphicspixmapitem.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QPoint>
#include <QScrollBar>
#include <QToolButton>
#include <QTransform>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_drag = false;
    // m_hotMode = false;
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    //缩放按钮菜单
    QAction *actZoomout = new QAction(QIcon(":/images/zoomout.png"), tr("放大"), this);
    QAction *actZoomin  = new QAction(QIcon(":/images/zoomin.png"), tr("缩小"), this);
    QAction *actZoom1   = new QAction(QIcon(":/images/zoom1.png"), tr("图像"), this);
    QAction *actZoomall = new QAction(QIcon(":/images/zoomall.png"), tr("所有"), this);
    connect(actZoomin, SIGNAL(triggered()), SLOT(slot_zoomin()));
    connect(actZoomout, SIGNAL(triggered()), SLOT(slot_zoomout()));
    connect(actZoom1, SIGNAL(triggered()), SLOT(slot_zoom1()));
    connect(actZoomall, SIGNAL(triggered()), SLOT(slot_zoomall()));

    QMenu *menu = new QMenu(this);
    menu->addAction(actZoomout);
    menu->addAction(actZoomin);
    menu->addAction(actZoom1);
    menu->addAction(actZoomall);
    CustomIconStyle *style = new CustomIconStyle;
    style->SetCustomSize(32);
    menu->setStyle(style);
    menu->setStyleSheet("QMenu{background:rgb(232,236,246);border:0px solid "
                        "rgb(232,236,246);border-radius: 3px;   /* 圆角 */color:black;}"
                        "QMenu::item::selected{background:rgb(84,115,232); color:white;}");

    m_zoomButton = new QToolButton(this);
    m_zoomButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_zoomButton->setToolTip(tr("缩放"));
    m_zoomButton->setIcon(QIcon(":/images/zoom.png"));
    m_zoomButton->setIconSize(QSize(32, 32));
    m_zoomButton->setPopupMode(QToolButton::InstantPopup);
    m_zoomButton->setMenu(menu);
    /*放大镜样式*/
    m_zoomButton->setStyleSheet(
        "QToolButton{background:rgb(232,236,246);border:0px solid rgb(232,236,246);padding: 2px "
        "2px 2px 2px;   /* 字体填衬 */border-radius: 3px;   /* 圆角 */color:black;}");

    // zoom layout
    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(m_zoomButton);
    zoomLayout->addStretch();

    // coordinate layout
    m_labelXY = new QLabel;
    QPalette pl;
    pl.setColor(QPalette::Text, Qt::blue);
    m_labelXY->setPalette(pl);
    QFont ft;
    ft.setPointSize(16);
    m_labelXY->setFont(ft);
    QHBoxLayout *coordLayout = new QHBoxLayout;
    coordLayout->addWidget(m_labelXY);
    coordLayout->addStretch();

    //整体布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(coordLayout);
    layout->addStretch();
    layout->addLayout(zoomLayout);
    layout->setMargin(30);
    layout->setSpacing(10);
    setLayout(layout);
}

void GraphicsView::addChart(QWidget *w)
{
    QLayout *layout   = this->layout();
    QLayoutItem *item = layout->itemAt(0);
    QLayout *ly1      = item->layout();
    ly1->addWidget(w);
}

void GraphicsView::setZoomButtonVisible(bool visible)
{
    m_zoomButton->setVisible(visible);
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && DragMode() == QGraphicsView::NoDrag)
    {
        // QGraphicsView的坐标
        QPoint point = event->pos();

#if 1
        QGraphicsItem *item = itemAt(point);

        if ((item && !item->isEnabled()) || !item) { emit mouseClicked(point); }

#else
        QGraphicsItem *item = itemAt(point);
        bool click          = false;
        if (!item) { click = true; }
        else
        {
            if (item->type() < QGraphicsItem::UserType) { click = true; }
        }
        if (click) emit mouseClicked(point);
#endif
    }
    else if (event->button() == Qt::MidButton)
    {
        m_drag = true;
        QApplication::setOverrideCursor(QCursor(Qt::OpenHandCursor));
        m_start = event->pos();
        return;
    }

    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->modifiers() == Qt::ALT)
    {
        QPointF pt  = mapToScene(event->pos());
        QString txt = QString("XY(%1,%2)\n").arg(pt.x(), 0, 'f', 0).arg(pt.y(), 0, 'f', 0);
        //
        QGraphicsScene *scene = this->scene();
        foreach (QGraphicsItem *item, scene->items())
        {
            if (item->type() == GraphicsPixmapItem::Type)
            {
                GraphicsPixmapItem *pixitem = qgraphicsitem_cast<GraphicsPixmapItem *>(item);
                QImage image                = pixitem->image();
                pt.rx() += 0.5; //因图像往左上方偏移过0.5，需要+0.5
                pt.ry() += 0.5;
                QPoint temp(static_cast<int>(pt.x()), static_cast<int>(pt.y()));

                if (!image.isNull()) //不为空
                {
                    if (image.valid(temp)) //坐标位置有效
                    {
                        QColor color = image.pixel(temp);
                        txt += QString("RGB(%1,%2,%3)")
                                   .arg(color.red())
                                   .arg(color.green())
                                   .arg(color.blue());
                    }
                }
                break;
            }
        }
        m_labelXY->setText(txt);
    }
    else
    {
        m_labelXY->setText("");
    }

    if (m_drag)
    {
        QPoint pt    = event->pos();
        QPoint delta = pt - m_start;
        translate(delta.x(), -delta.y());
        m_start = pt;
        // return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MidButton && m_drag)
    {
        m_drag = false;
        QApplication::restoreOverrideCursor();
        return;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = -event->delta() / 8.0;
    double numSteps   = numDegrees / 15.0;
    double factor     = std::pow(1.125, numSteps);
    scale(factor, factor);

    event->accept();
}

void GraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    //缩放比例数据显示
    QTransform &&tf = transform();
    qreal factor    = tf.m11();
    m_zoomButton->setText(QString::number(factor, 'f', 1));

    QGraphicsView::drawForeground(painter, rect);
}

void GraphicsView::slot_zoomin()
{
    scale(0.9, 0.9);
}

void GraphicsView::slot_zoomout()
{
    scale(1.1, 1.1);
}

void GraphicsView::slot_zoom1()
{
    QGraphicsScene *scene = this->scene();
    foreach (QGraphicsItem *item, scene->items())
    {
        if (item->type() == QGraphicsPixmapItem::Type || item->type() == GraphicsPixmapItem::Type)
        {
            setSceneRect(item->boundingRect());
            fitInView(item, Qt::KeepAspectRatio);
            break;
        }
    }
}

void GraphicsView::slot_zoomall()
{
    QGraphicsScene *scene = this->scene();
    //  setSceneRect(scene->itemsBoundingRect());
    //  fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

    QRectF rect;
    foreach (QGraphicsItem *item, scene->items())
        rect = rect.united(item->boundingRect());

    setSceneRect(rect);
    fitInView(rect, Qt::KeepAspectRatio);
}
