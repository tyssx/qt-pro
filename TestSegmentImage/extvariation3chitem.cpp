#include "extvariation3chitem.h"

//#include "common.h"
//#include "variation3chpage.h"
#include "dugroupdefect.h"
#include "widget.h"

#include <QApplication>
#include <QCursor>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QMessageBox>
#include <QPainter>
#include <QProgressDialog>
#include <QStringList>
#include <QStyleOptionGraphicsItem>
#include <QThread>
#include <QtMath>

static int serial   = 0;
const int kPickSize = 15; //拾取精度/控制点大小

qreal ptToLine(const QLineF &line, const QPointF &pt)
{
    QLineF l(line.p1(), pt);
    qreal angle = line.angleTo(l);
    qreal len   = l.length() * qSin(angle * M_PI / 180);
    return qAbs(len);
}

ExtVariation3ChItem::ExtVariation3ChItem()
{
    m_display = static_cast<AbstractDisplay *>(&m_result);

    //    setTask(new ExtVariation3ChTask);
    m_task = new ExtVariation3ChTask;
    setAcceptHoverEvents(false);

    connect(m_task,
        &ExtVariation3ChTask::sendEdges,
        this,
        [&](CxPointArray roiArray, CxPointArray segment, CxPointArray CenterArea) {
            qDebug() << " ExtVariation3ChItem emit sendEdges";
            emit sendEdges(roiArray, segment, CenterArea);
        });
    //缺陷算法参数
    //    m_property = new Variation3ChPage();
    //    m_property->setParam(m_param, m_simpParam);
    //    connect(m_property, &Variation3ChPage::paramChange, [this](ExtVariation3ChTask::Param pm)
    //    {
    //        m_param = pm;
    //        progressDetect();
    //    });

    //    connect(m_property, &Variation3ChPage::simpParamChange,
    //    [this](ExtVariation3ChTask::SimpParam spm) {
    //        m_simpParam = spm;
    //        m_task->setParam(&m_simpParam, sizeof(m_simpParam));
    //        m_task->simplifyRegion();
    //    });

    //    connect(m_property, &Variation3ChPage::regionFlash, [this](int index) {

    //    });
}

ExtVariation3ChItem::~ExtVariation3ChItem()
{
    qDebug() << "Esvi Variation3Ch QGraphicsItem destory!";
    //    delete m_property;
}

QRectF ExtVariation3ChItem::boundingRect() const
{
    QRectF rect = m_polygon.boundingRect();

    //    rect |= m_result.boundingRect();
    qreal d = (kPickSize + 1) / m_factor / 2;
    rect.adjust(-d, -d, d, d);

    return rect;
}

void ExtVariation3ChItem::paint(
    QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    const QTransform &tf = painter->transform();
    m_factor             = tf.m11();
    if (m_count == 0) return;

    if (m_buildState & Success) m_result.paint(painter, option, widget);
    //    if ((m_buildState & Ongoing) || m_hover || option->state & QStyle::State_Selected)
    //    {
    QPen pen;
    pen.setCosmetic(true);

    //控制点
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    pen.setWidth(kPickSize);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    for (int i = 0; i < qMin(m_count, Num - 1); i++) painter->drawPoint(m_ctrl[i]);

    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawPolygon(m_polygon);
    //    }
    // painter->drawRect(boundingRect());
}

QPainterPath ExtVariation3ChItem::shape() const
{
    return m_shape;
}

// void ExtVariation3ChItem::setDefaultName()
//{
//    setData(TASK_NAME, tr("三通道缺陷对象%1").arg(++serial));

//    QByteArray name = data(TASK_NAME).toString().toLocal8Bit();
//    m_task->setTaskName(name.toStdString());
//}

void ExtVariation3ChItem::editorDetect()
{
    //    LoadVariationTemplate();

    //交互参数
    ExtVariation3ChTask::Roi roi;
    QLineF l(m_ctrl[0], m_ctrl[1]);
    QPointF ct = l.center();
    roi.x      = float(ct.x());
    roi.y      = float(ct.y());
    roi.width  = float(m_width);        //未使用？
    roi.height = float(l.length() / 2); //未使用？
    roi.angle  = float(l.angle());

    //    getAllUnionRegion(roi);
    m_task->setRoi(&roi, sizeof(roi));

    //算法参数
    m_param.model_id = m_modelId;
    m_task->setParam(&m_param, sizeof(m_param));

    //运行检测
    //    m_task->resetCoord();
    m_task->detect(2);
}

// QWidget *ExtVariation3ChItem::property()
//{
//    return m_property;
//}

void ExtVariation3ChItem::buildFromCtrlpoint(QPointF pt, /* DataUnit *du,*/ bool isTrack)
{
    if (m_buildState & Init) m_buildState = Ongoing;
    if (m_buildState & Success) return;

    for (int i = m_count; i < Num; i++) { m_ctrl[i] = pt; }
    if (!isTrack) ++m_count; //动态跟踪点，
    createShape();
}

void ExtVariation3ChItem::updateOut()
{
    DuGroupDefect *du = m_task->output();
    du->updateDisplay(m_display);
}

void ExtVariation3ChItem::createShape(bool calWidth)
{
    if (m_buildState <= Ongoing && m_count >= Num)
    {
        m_buildState = checkIn() ? Success : Failed;
        setAcceptHoverEvents(true);
    }

    if (calWidth)
    {
        //计算控制带宽
        QLineF l(m_ctrl[0], m_ctrl[1]);
        qreal alfa = l.angleTo(QLineF(m_ctrl[0], m_ctrl[2]));
        qreal beta = l.angle() + (alfa < 180 ? 90 : -90);
        m_width    = ptToLine(l, m_ctrl[2]);

        m_line = QLineF::fromPolar(2 * m_width, beta);
        m_line.translate(l.center() - m_line.center());
        // qDebug() << "center:" << l.center() - m_line.center() << "m_width:" << m_width;
    }
    else
    {
        //保持宽度不变，重新计算m_ctrl[Num-1]
        QLineF l(m_ctrl[0], m_ctrl[1]);
        m_line.translate(l.center() - m_line.center());
        qreal alfa = l.angleTo(m_line);
        qreal beta = l.angle() + (alfa < 180 ? 90 : -90);
        m_line.setAngle(beta);
        m_ctrl[2] = m_line.p2();
    }

    //外框
    QLineF l(m_ctrl[0], m_ctrl[1]);
    qreal theta = qDegreesToRadians(l.angle() + 90);
    qreal dx    = m_width * qCos(theta);
    qreal dy    = m_width * qSin(theta);
    QLineF l1   = l.translated(dx, -dy);
    QLineF l2   = l.translated(-dx, +dy);
    m_polygon.clear();
    m_polygon << l1.p1() << l1.p2() << l2.p2() << l2.p1() << l1.p1();

    //拾取形状
    QPainterPathStroker ps;
    QPainterPath path;
    path.addPolygon(m_polygon);
    qreal dd = kPickSize / m_factor; //+1
    ps.setWidth(dd);
    m_shape = ps.createStroke(path);
    m_shape = m_shape.united(path);

    prepareGeometryChange();
}

void ExtVariation3ChItem::setCurrent(bool hideResult)
{
    QGraphicsScene *scene = this->scene();
    scene->clearSelection();
    this->setSelected(true);

    //    if (hideResult)
    //    {
    //        if (m_display) m_display->clear();
    //        prepareGeometryChange();
    //    }
}

void ExtVariation3ChItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_start     = event->scenePos();
        m_transform = true;

        setCurrent();
    }
    else
    {
        QGraphicsItem::mousePressEvent(event);
    }
}

void ExtVariation3ChItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_transform)
    {
        QPointF pt    = event->scenePos();
        QPointF delta = pt - m_start;
        QLineF tm(m_start, pt);
        m_start = pt;
        switch (m_direction)
        {
        case 0:
            m_ctrl[0] = pt;
            createShape(false);
            break;
        case 1:
            m_ctrl[1] = pt;
            createShape(false);
            break;
        case 2:
        case 3:
        {
            // m_ctrl[0], m_ctrl[1]所连直线为基线，
            //计算基线之法线方向偏移量
            QLineF line(m_ctrl[0], m_ctrl[1]);
            qreal theta = -(tm.angleTo(line) + 90) * M_PI / 180;
            qreal dis   = tm.length() * cos(theta) / 2;

            //基线按法线方向移动，距离为偏移量的一半
            theta    = -(line.angle() + 90) * M_PI / 180;
            qreal dx = dis * qCos(theta);
            qreal dy = dis * qSin(theta);
            line.translate(dx, dy);
            m_ctrl[0] = line.p1();
            m_ctrl[1] = line.p2();

            m_ctrl[2] = pt;
            createShape();
        }
        break;
        case 5:
            for (int i = 0; i < Num; i++) m_ctrl[i] += delta;
            createShape();
            break;
        default: break;
        }
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ExtVariation3ChItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_transform = false;
    QGraphicsItem::mouseReleaseEvent(event);
    editorDetect();
    emit detectChanged(this);
}

void ExtVariation3ChItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = true;
    this->setCursor(Qt::SizeAllCursor);
    QGraphicsItem::hoverEnterEvent(event);
}

void ExtVariation3ChItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QPointF pos = event->pos();

    //找到最近的端点
    m_direction = 5;
    qreal min   = kPickSize / m_factor / 2 + 1;
    qreal dis[4];
    QPointF dt;

    //二个端点
    for (int i = 0; i < Num - 1; i++)
    {
        dt     = pos - m_ctrl[i];
        dis[i] = hypot(dt.x(), dt.y());
    }

    //两条边界
    dis[2] = ptToLine(QLineF(m_polygon.at(0), m_polygon.at(1)), pos);
    dis[3] = ptToLine(QLineF(m_polygon.at(2), m_polygon.at(3)), pos);

    for (int i = 0; i < 4; i++)
    {
        if (dis[i] < min)
        {
            min         = dis[i];
            m_direction = i;
        }
    }

    //根据位置不同可设置不同的光标
    switch (m_direction)
    {
    case 0:
    case 1: this->setCursor(Qt::PointingHandCursor); break;
    case 2:
    case 3: this->setCursor(Qt::SplitHCursor); break;
    case 5: this->setCursor(Qt::SizeAllCursor); break;
    default: this->setCursor(Qt::ArrowCursor); break;
    }

    QGraphicsItem::hoverMoveEvent(event);
}

void ExtVariation3ChItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hover = false;
    this->setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

void ExtVariation3ChItem::setImgHandle(INT64 handle, Widget *widget)
{
    m_widget = widget;
    connect(this, &ExtVariation3ChItem::sendEdges, m_widget, &Widget::slot_getEdges);
    if (m_task) { m_task->imgHandle = handle; }
}
