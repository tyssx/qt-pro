#include "transportset.h"

#include "arrowsbutton.h"
#include "autotransport.h"
#include "ui_transportset.h"

#include <QAction>
#include <QDebug>
#include <QHBoxLayout>
#include <QTimer>

#define MOVE_DIS 1
#define INTERVAL 50

TransportSet::TransportSet(const DeviceManager *device, QWidget *parent)
    : QDialog(parent), ui(new Ui::TransportSet), m_device(device)
{
    ui->setupUi(this);
    m_action = new QAction(tr("物料运输设置"), this);
    QIcon icon;
    icon.addFile(":/images/transport.png", QSize(), QIcon::Normal, QIcon::Off);
    m_action->setIcon(icon);

    ui->pushButtonLeft->init(ArrowsButton::left);
    ui->pushButtonRight->init(ArrowsButton::right);
    ui->pushButtonFront->init(ArrowsButton::front);
    ui->pushButtonBack->init(ArrowsButton::back);

    ui->widgetLoadingRocker->init(m_device, RockerWidget::loading);
    ui->widgetUnloadingRocker->init(m_device, RockerWidget::unloading);

    m_motion = m_device->motion();

    m_autoTransport = new AutoTransport(m_device);

    uint32 value = 0;
    m_motion->readOutBit(DeviceManager::OUT6, value);
    on_pushButtonAirSwitch_toggled(value);
    //        setButtonStatus(false);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &TransportSet::slot_timeout);
    QTimer *timerL = new QTimer();
    QTimer *timerR = new QTimer();
    QTimer *timerF = new QTimer();
    QTimer *timerB = new QTimer();

    connect(timerL, &QTimer::timeout, this, &TransportSet::slot_leftTimeout);
    connect(timerR, &QTimer::timeout, this, &TransportSet::slot_rightTimeout);
    connect(timerF, &QTimer::timeout, this, &TransportSet::slot_frontTimeout);
    connect(timerB, &QTimer::timeout, this, &TransportSet::slot_backTimeout);
    connect(ui->pushButtonLeft, &ArrowsButton::pressed, this, [=]() { /*timerL->start(50);*/
        leftIsPress = true;
        m_timeId    = startTimer(INTERVAL);

    });
    connect(ui->pushButtonLeft, &ArrowsButton::released, this, [=]() {
        //        timerL->stop();
        //        m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
        leftIsPress = false;
        killTimer(m_timeId);
        m_timeId = 0;
        if (m_motion->isAvailable())
            m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
    });
    connect(ui->pushButtonRight, &ArrowsButton::pressed, this, [=]() { /*timerR->start(50);*/
        rightIsPress = true;
        m_timeId     = startTimer(INTERVAL);
    });
    connect(ui->pushButtonRight, &ArrowsButton::released, this, [=]() {
        //        timerR->stop();
        //        m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
        rightIsPress = false;
        killTimer(m_timeId);
        m_timeId = 0;
        if (m_motion->isAvailable())
            m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
    });
    connect(ui->pushButtonFront, &ArrowsButton::pressed, this, [=]() { timerF->start(50); });
    connect(ui->pushButtonFront, &ArrowsButton::released, this, [=]() {
        timerF->stop();
        m_motion->stop(m_device->axisInfo().railwidth, CMotionCtrlDev::STOP_IMMEDIATE);
    });
    connect(ui->pushButtonBack, &ArrowsButton::pressed, this, [=]() { timerB->start(50); });
    connect(ui->pushButtonBack, &ArrowsButton::released, this, [=]() {
        timerB->stop();
        m_motion->stop(m_device->axisInfo().railwidth, CMotionCtrlDev::STOP_IMMEDIATE);
    });
}

TransportSet::~TransportSet()
{
    delete ui;
}

void TransportSet::closeIO()
{
    on_pushButtonAirSwitch_toggled(false);
    on_pushButtonTransport_toggled(false);
}

void TransportSet::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    timer->start(500);
}

void TransportSet::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)
    timer->stop();
}

void TransportSet::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timeId)
    {
        if (m_motion && m_motion->isAvailable())
        {
            if (leftIsPress)
            {
                double pos;
                m_motion->getTargetPosition(m_device->axisInfo().transport, pos);
                pos -= MOVE_DIS;
                m_motion->changeTargetPosition(m_device->axisInfo().transport, pos);
            }
            if (rightIsPress)
            {
                double pos;
                m_motion->getTargetPosition(m_device->axisInfo().transport, pos);
                pos += MOVE_DIS;
                m_motion->changeTargetPosition(m_device->axisInfo().transport, pos);
            }
        }
    }
}

void TransportSet::setButtonStatus(bool flag)
{
    ui->pushButtonWaitResist->setEnabled(flag);
    ui->pushButtonArriveResist->setEnabled(flag);
    ui->pushButtonMaterBoard->setEnabled(flag);
    ui->pushButtonLoadingPush->setEnabled(flag);
    ui->pushButtonUnloadingPush->setEnabled(flag);
    ui->pushButtonUnloadingBoard->setEnabled(flag);
}

void TransportSet::on_pushButtonAirSwitch_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT6, 1);
        ui->pushButtonAirSwitch->setText("关闭气压");
        setButtonStatus(true);
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT6, 0);
        ui->pushButtonAirSwitch->setText("打开气压");
        setButtonStatus(false);
        on_pushButtonWaitResist_toggled(false);
        on_pushButtonArriveResist_toggled(false);
        on_pushButtonMaterBoard_toggled(false);
        on_pushButtonLoadingPush_toggled(false);
        on_pushButtonUnloadingPush_toggled(false);
        on_pushButtonUnloadingBoard_toggled(false);
        ui->pushButtonWaitResist->setChecked(false);
        ui->pushButtonArriveResist->setChecked(false);
        ui->pushButtonMaterBoard->setChecked(false);
        ui->pushButtonLoadingPush->setCheckable(false);
        ui->pushButtonUnloadingPush->setCheckable(false);
        ui->pushButtonUnloadingBoard->setCheckable(false);
    }
}

void TransportSet::on_pushButtonWaitResist_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT7, 1);
        ui->pushButtonWaitResist->setText("关闭待料阻挡");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT7, 0);
        ui->pushButtonWaitResist->setText("打开待料阻挡");
    }
}

void TransportSet::on_pushButtonArriveResist_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT8, 1);
        ui->pushButtonArriveResist->setText("关闭到位阻挡");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT8, 0);
        ui->pushButtonArriveResist->setText("打开到位阻挡");
    }
}

void TransportSet::on_pushButtonMaterBoard_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT9, 1);
        ui->pushButtonMaterBoard->setText("降落物料顶板");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT9, 0);
        ui->pushButtonMaterBoard->setText("抬升物料顶板");
    }
}

void TransportSet::on_pushButtonLoadingPush_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT11, 1);
        ui->pushButtonLoadingPush->setText("收回上料料夹");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT11, 0);
        ui->pushButtonLoadingPush->setText("推出上料料夹");
    }
}

void TransportSet::on_pushButtonUnloadingPush_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT12, 1);
        ui->pushButtonUnloadingPush->setText("收回下料料夹");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT12, 0);
        ui->pushButtonUnloadingPush->setText("推入下料料夹");
    }
}

void TransportSet::on_pushButtonUnloadingBoard_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT13, 1);
        ui->pushButtonUnloadingBoard->setText("下料夹降落顶升");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT13, 0);
        ui->pushButtonUnloadingBoard->setText("下料夹抬升顶升");
    }
}

void TransportSet::on_pushButtonTransport_toggled(bool checked)
{
    if (checked)
    {
        ui->pushButtonTransport->setText("停止物料运输");
        m_motion->move(m_device->axisInfo().transport, CMotionCtrlDev::DIR_POSITIVE);
    }
    else
    {
        ui->pushButtonTransport->setText("启动物料运输");
        m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
    }
}

void TransportSet::slot_transportAxisMove()
{
    ArrowsButton *button = qobject_cast<ArrowsButton *>(sender());
    if (button == ui->pushButtonLeft)
    {
        if (m_motion->isAvailable())
        {
            while (leftIsPress)
            { m_motion->move(m_device->axisInfo().transport, -1, CMotionCtrlDev::POS_RELATIVE); }
        }
    }
}

void TransportSet::slot_railwidthAxisMove()
{
}

void TransportSet::slot_timeout()
{
    if (m_motion->isAvailable())
    {
        double pos = 0.0;
        m_motion->getPosition(m_device->axisInfo().railwidth, pos);
        ui->lineEditRailCurPos->setText(QString::number(pos));
    }
}

void TransportSet::slot_leftTimeout()
{
    if (m_motion->isAvailable())
    { m_motion->move(m_device->axisInfo().transport, -0.5, CMotionCtrlDev::POS_RELATIVE); }
}

void TransportSet::slot_rightTimeout()
{
    if (m_motion->isAvailable())
    { m_motion->move(m_device->axisInfo().transport, 0.5, CMotionCtrlDev::POS_RELATIVE); }
}

void TransportSet::slot_frontTimeout()
{
    if (m_motion->isAvailable())
    { m_motion->move(m_device->axisInfo().railwidth, -0.5, CMotionCtrlDev::POS_RELATIVE); }
}

void TransportSet::slot_backTimeout()
{
    if (m_motion->isAvailable())
    { m_motion->move(m_device->axisInfo().railwidth, 0.5, CMotionCtrlDev::POS_RELATIVE); }
}

void TransportSet::on_pushButtonLoading_clicked()
{
    m_autoTransport->slot_loading();
}

void TransportSet::on_pushButtonUnloading_clicked()
{
    m_autoTransport->slot_unloading();
}
