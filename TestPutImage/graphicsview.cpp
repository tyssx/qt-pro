#include "graphicsview.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QLabel>
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
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QToolButton *menuButton = new QToolButton(this);
    connect(menuButton, &QToolButton::clicked, this, &GraphicsView::slot_zoomall);
    menuButton->setToolTip(tr("自适应"));
    menuButton->setIcon(QIcon(":/images/zoomall.png"));
    menuButton->setIconSize(QSize(32, 32));
    menuButton->setAutoFillBackground(true);
    // zoom layout
    QHBoxLayout *zoomLayout = new QHBoxLayout;
    zoomLayout->addWidget(menuButton);
    zoomLayout->addStretch();
    // coordinate layout
    m_labelXY = new QLabel;
    QPalette pl;
    pl.setColor(QPalette::Text, Qt::blue);
    m_labelXY->setPalette(pl);
    //  m_labelXY->setAutoFillBackground(true);
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

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    double numDegrees = event->delta() / 8.0;
    double numSteps   = numDegrees / 15.0;
    double factor     = std::pow(1.125, numSteps);
    scale(factor, factor);
}

void GraphicsView::slot_zoomall()
{
    QGraphicsScene *scene = this->scene();
    foreach (QGraphicsItem *item, scene->items())
    {
        if (item->type() == QGraphicsPixmapItem::Type)
        {
            setSceneRect(item->boundingRect());
            fitInView(item, Qt::KeepAspectRatio);
            break;
        }
    }
}
