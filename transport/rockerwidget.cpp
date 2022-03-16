#include "rockerwidget.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#define interval 200

RockerWidget::RockerWidget(QWidget *parent) : QWidget(parent)
{
    m_flag     = false;
    m_maxSpeed = 20;
    m_smallDis = 1.0;
}

RockerWidget::~RockerWidget()
{
    qDebug() << "RockerWidget destroy !";
}

void RockerWidget::init(const DeviceManager *device, int axisType)
{
    m_device   = device;
    m_axisType = axisType;
    m_motion   = m_device->motion();

    if (m_axisType == 0)
        m_axis = m_device->axisInfo().loading;
    else if (m_axisType == 1)
    {
        m_axis = m_device->axisInfo().unloading;
    }
}

void RockerWidget::resizeEvent(QResizeEvent *event)
{
    w       = width();
    qreal h = height() / 2.0;
    qDebug() << "w=" << width() << "h=" << height();
    m_radius      = h * 0.65;
    m_rHand       = h - m_radius;
    m_centre.rx() = width() / 2.0;
    m_centre.ry() = height() / 2.0;
    m_pos         = m_centre;

    // z轴摇杆区
    QRectF rect(0, 0, w * 0.9, m_radius * 2 * 0.9);
    rect.moveCenter(m_centre);
    m_path[0] = QPainterPath();
    m_path[0].addRect(rect);

#if 0
    QPainterPathStroker ps;
    ps.setWidth(10);
    ps.setCapStyle(Qt::FlatCap);

    // z轴微调区域扇形
    QRect rtpie(0, 0, w * 0.8, m_radius * 2);
    rtpie.moveCenter(QPoint(m_centre.rx() - 1, m_centre.ry()));

    QPainterPath upper;
    upper.arcMoveTo(rtpie, 50);
    upper.arcTo(rtpie, 50, 80);
    m_path[1] = ps.createStroke(upper);

    QPainterPath down;
    down.arcMoveTo(rtpie, 230);
    down.arcTo(rtpie, 230, 80);
    m_path[2] = ps.createStroke(down);

#else
    QPointF pt;
    // z轴微调区域三角形
    QPolygonF upper;
    pt = {m_centre.x(), m_centre.y() - m_radius};
    upper << pt + QPointF{-m_rHand, 0} << pt + QPointF{m_rHand, 0} << pt + QPointF{0, -m_rHand};
    m_path[1] = QPainterPath();
    m_path[1].addPolygon(upper);

    QPolygonF down;
    pt = {m_centre.x(), m_centre.y() + m_radius};
    down << pt + QPointF{-m_rHand, 0} << pt + QPointF{m_rHand, 0} << pt + QPointF{0, m_rHand};
    m_path[2] = QPainterPath();
    m_path[2].addPolygon(down);
#endif
    QWidget::resizeEvent(event);
}

void RockerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect(0, 0, w * 0.9, m_radius * 2 * 0.9);
    rect.moveCenter(m_centre);

    QBrush brush(QColor(0, 180, 255), Qt::SolidPattern);
    QPen pen(QColor(Qt::black), 1, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.setBrush(brush);

    //画微调扇形
    painter.drawPath(m_path[1]);
    painter.drawPath(m_path[2]);

    //画xy轴摇杆背景
    pen.setColor(QColor(0, 0, 0));
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    QPixmap pixmap;
    pixmap.load(":/images/ud.png");
    painter.drawPixmap(rect, pixmap, QRectF(0, 0, pixmap.width(), pixmap.height()));
    painter.drawRect(rect);

    //画摇杆
    //    QBrush brush(Qt::darkRed, Qt::SolidPattern);
    brush.setColor(Qt::darkRed);
    //    QPen pen(brush, m_radius / 2, Qt::SolidLine, Qt::RoundCap);
    pen = QPen(brush, m_radius / 2, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.drawLine(m_centre, m_pos);

    //画xy轴摇把
    qreal radius = m_rHand;
    QRectF br(0, 0, w, w);
    br.moveCenter(m_pos);
    QRadialGradient rg(m_pos, radius, m_pos - QPointF(radius / 2, radius / 2));
    rg.setColorAt(0, QColor(200, 200, 200));
    //    rg.setColorAt(0.25, QColor(50, 200, 50));
    //    rg.setColorAt(0.4, QColor(0, 150, 0));
    //    rg.setColorAt(0.6, QColor(0, 120, 0));
    //    rg.setColorAt(1, QColor(0, 80, 0));
    rg.setColorAt(0.25, QColor(0, 200, 255));
    rg.setColorAt(0.4, QColor(0, 150, 255));
    rg.setColorAt(0.6, QColor(0, 120, 255));
    rg.setColorAt(1, QColor(0, 80, 255));
    painter.setPen(Qt::NoPen);
    painter.setBrush(rg);
    painter.drawEllipse(br);
}

void RockerWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_flag = false;

        //取主轴原始速度
        if (m_motion && m_motion->isAvailable())
        {
            m_motion->getSpeed(m_axis, m_speed);

            if (m_axisType == loading)
            {
                m_maxSpeed = m_device->axisInfo().loadmaxspeed;
                m_smallDis = m_device->axisInfo().loadsmalldis;
            }
            else if (m_axisType == unloading)
            {
                m_maxSpeed = m_device->axisInfo().unloadmaxspeed;
                m_smallDis = m_device->axisInfo().unloadsmalldis;
            }

            if (m_maxSpeed > m_speed.speed_) m_maxSpeed = m_speed.speed_;
        }

        //位置判别
        for (int i = 0; i < 3; ++i)
        {
            if (m_path[i].contains(event->pos()))
            {
                emit rockerTriggered();

                switch (i)
                {
                case 0: m_flag = true; break;
                case 1:
                    if (m_axisType == loading)
                    {
                        uint32 value = 0;
                        m_motion->readInBit(DeviceManager::IN15, value);
                        if (value == 1) continue;
                    }
                    else if (m_axisType == unloading)
                    {
                        uint32 value = 0;
                        m_motion->readInBit(DeviceManager::IN39, value);
                        if (value == 1) continue;
                    }
                    m_motion->move(m_axis, -1 * m_smallDis, CMotionCtrlDev::POS_RELATIVE);
                    qDebug() << "axis" << m_axis << "upper--";
                    break;
                case 2:
                    if (m_axisType == loading)
                    {
                        uint32 value = 0;
                        m_motion->readInBit(DeviceManager::IN15, value);
                        if (value == 1) continue;
                    }
                    else if (m_axisType == unloading)
                    {
                        uint32 value = 0;
                        m_motion->readInBit(DeviceManager::IN39, value);
                        if (value == 1) continue;
                    }
                    m_motion->move(m_axis, m_smallDis, CMotionCtrlDev::POS_RELATIVE);
                    qDebug() << "axis" << m_axis << "down--";
                    break;
                }
                break;
            }
        }

        //按下了摇杆
        if (m_flag)
        {
            m_pos = event->pos();
            m_line.setPoints(m_centre, m_pos);
            if (m_line.length() > m_radius) m_line.setLength(m_radius); //长度限制

            //纠正方向，与Y轴平行
            qreal theta = m_line.angle();
            if (theta >= 0 && theta <= 180)
                m_line.setAngle(90);
            else if (theta > 180 && theta <= 360)
            {
                m_line.setAngle(270);
            }
            m_pos = m_line.p2();
            update();
            m_timeId = startTimer(interval);
        }
    }
}

void RockerWidget::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && m_flag)
    {
        m_pos = event->pos();
        m_line.setPoints(m_centre, m_pos);
        if (m_line.length() > m_radius) m_line.setLength(m_radius); //长度限制

        qreal theta = m_line.angle();
        //纠正方向，与Y轴平行
        if (theta >= 0 && theta <= 180)
            m_line.setAngle(90);
        else if (theta > 180 && theta <= 360)
        {
            m_line.setAngle(270);
        }
        m_pos = m_line.p2();
        update();
    }
}

void RockerWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if (m_flag)
    {
        m_flag = false;
        killTimer(m_timeId);
        m_timeId = 0;
        if (m_motion && m_motion->isAvailable())
        {
            //停止及速度还原
            m_motion->stop();
            m_motion->setSpeed(m_axis, m_speed);
        }

        m_pos = m_centre;
        update();
    }

    return;
}

void RockerWidget::timerEvent(QTimerEvent *event)
{
    // qDebug() << "Timer ID:" << event->timerId();

    if (event->timerId() == m_timeId)
        if (m_motion && m_motion->isAvailable())
        {
            if (m_flag)
            {
                //上料夹，下料夹伸开时，轴不能动
                if (m_axisType == loading)
                {
                    uint32 value = 0;
                    m_motion->readInBit(DeviceManager::IN15, value);
                    if (value == 1) return;
                }
                else if (m_axisType == unloading)
                {
                    uint32 value = 0;
                    m_motion->readInBit(DeviceManager::IN39, value);
                    if (value == 1) return;
                }

                //角度计算
                qreal theta = m_line.angle() * M_PI /
                    180; //转
                         //速度和摇杆成正比 速度*(线长/工作半径）线长越长速度越快
                         //                qreal v = m_speed.speed_ * m_line.length() / m_radius;
                qreal v = m_maxSpeed * m_line.length() / m_radius;
                //            qDebug() << "V is" << v;
                //计算每个间隔距离
                qreal s = v * interval / 1000 *
                    2; //速度为20时，每次能提高的最大距离s为2，*2是为了加大距离
                //            if (s < 2) s = 2; //如果S太小，则运动控制会有BUG出现，需联系厂家处理
                //在线变速
                //            m_motion->changeSpeed(m_z, v);

                double pos;
                m_motion->getTargetPosition(m_axis, pos); //读取正在运动的目标位置
                pos -= s * qSin(theta); // Z轴0位置在上，>0在下，摇杆线为90度时要减，270度时要加

                m_motion->changeTargetPosition(m_axis, pos); //在线变位
            }
        }
}
