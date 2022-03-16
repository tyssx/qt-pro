#include "transportset.h"

#include "CStorageInterface.h"
#include "arrowsbutton.h"
#include "ui_transportset.h"

#include <QAction>
#include <QButtonGroup>
#include <QDebug>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QThread>
#include <QTimer>

//#define MOVE_DIS 1
#define INTERVAL 50

TransportSet::TransportSet(const DeviceManager *device, QWidget *parent)
    : QDialog(parent), ui(new Ui::TransportSet), m_device(device)
{
    ui->setupUi(this);
    m_action = new QAction(tr("物料运输设置"), this);
    QIcon icon;
    icon.addFile(":/images/transport.png", QSize(), QIcon::Normal, QIcon::Off);
    m_action->setIcon(icon);

    //按键初始化
    ui->pushButtonLeft->init(ArrowsButton::left);
    ui->pushButtonRight->init(ArrowsButton::right);
    ui->pushButtonFront->init(ArrowsButton::front);
    ui->pushButtonBack->init(ArrowsButton::back);

    //上料摇杆初始化
    ui->widgetLoadingRocker->init(m_device, RockerWidget::loading);
    ui->widgetUnloadingRocker->init(m_device, RockerWidget::unloading);

    m_motion = m_device->motion();
    m_store  = m_device->store();

    m_autoTransport = new AutoTransport(m_device);

    qRegisterMetaType<AutoTransport::MagazinePos>("AutoTransport::MagazinePos");

    uint32 value = 0;
    m_motion->readOutBit(DeviceManager::OUT6, value);
    on_pushButtonAirSwitch_toggled(value);
    //        setButtonStatus(false);

    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &TransportSet::slot_timeout);
    //    QTimer *timerL = new QTimer();
    //    QTimer *timerR = new QTimer();
    //    QTimer *timerF = new QTimer();
    //    QTimer *timerB = new QTimer();

    //    connect(timerL, &QTimer::timeout, this, &TransportSet::slot_leftTimeout);
    //    connect(timerR, &QTimer::timeout, this, &TransportSet::slot_rightTimeout);
    //    connect(timerF, &QTimer::timeout, this, &TransportSet::slot_frontTimeout);
    //    connect(timerB, &QTimer::timeout, this, &TransportSet::slot_backTimeout);

    QButtonGroup *transGroup = new QButtonGroup;
    transGroup->addButton(ui->pushButtonLeft);
    transGroup->addButton(ui->pushButtonRight);
    transGroup->addButton(ui->pushButtonFront);
    transGroup->addButton(ui->pushButtonBack);
    connect(transGroup,
        static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonPressed),
        this,
        &TransportSet::slot_buttonPress);
    connect(transGroup,
        static_cast<void (QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonReleased),
        this,
        &TransportSet::slot_buttonRelease);
    /*
        connect(ui->pushButtonLeft, &ArrowsButton::pressed, this, [=]() {
            // timerL->start(50);
            leftIsPress = true;
            m_timeId    = startTimer(INTERVAL);
        });
        connect(ui->pushButtonLeft, &ArrowsButton::released, this, [=]() {
            //        timerL->stop();
            //        m_motion->stop(m_device->axisInfo().transport,
       CMotionCtrlDev::STOP_IMMEDIATE); leftIsPress = false; killTimer(m_timeId); m_timeId = 0; if
       (m_motion->isAvailable()) m_motion->stop(m_device->axisInfo().transport,
       CMotionCtrlDev::STOP_IMMEDIATE);
        });
        connect(ui->pushButtonRight, &ArrowsButton::pressed, this, [=]() {
            // timerR->start(50);
            rightIsPress = true;
            m_timeId     = startTimer(INTERVAL);
        });
        connect(ui->pushButtonRight, &ArrowsButton::released, this, [=]() {
            //        timerR->stop();
            //        m_motion->stop(m_device->axisInfo().transport,
       CMotionCtrlDev::STOP_IMMEDIATE); rightIsPress = false; killTimer(m_timeId); m_timeId = 0; if
       (m_motion->isAvailable()) m_motion->stop(m_device->axisInfo().transport,
       CMotionCtrlDev::STOP_IMMEDIATE);
        });*/
    //    connect(ui->pushButtonFront, &ArrowsButton::pressed, this, [=]() { timerF->start(50); });
    //    connect(ui->pushButtonFront, &ArrowsButton::released, this, [=]() {
    //        timerF->stop();
    //        m_motion->stop(m_device->axisInfo().railwidth, CMotionCtrlDev::STOP_IMMEDIATE);
    //    });
    //    connect(ui->pushButtonBack, &ArrowsButton::pressed, this, [=]() { timerB->start(50); });
    //    connect(ui->pushButtonBack, &ArrowsButton::released, this, [=]() {
    //        timerB->stop();
    //        m_motion->stop(m_device->axisInfo().railwidth, CMotionCtrlDev::STOP_IMMEDIATE);
    //    });

    connect(this,
        &TransportSet::updateMagazinePos,
        m_autoTransport,
        &AutoTransport::slot_updatePos,
        Qt::QueuedConnection);
    connect(this,
        &TransportSet::detectBoard,
        m_autoTransport,
        &AutoTransport::slot_detectBoard,
        Qt::BlockingQueuedConnection);
    connect(this,
        &TransportSet::startRun,
        m_autoTransport,
        &AutoTransport::slot_run,
        Qt::QueuedConnection);
    connect(this,
        &TransportSet::restartLoading,
        m_autoTransport,
        &AutoTransport::slot_restartLoading,
        Qt::QueuedConnection);
    connect(this,
        &TransportSet::endDetect,
        m_autoTransport,
        &AutoTransport::slot_unloading,
        Qt::QueuedConnection);

    initTransParam(); //初始化料盒参数
    initPushButton(); //初始化料盒按键
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
    timer->start(50);
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
                //                pos -= MOVE_DIS;
                pos -= m_device->axisInfo().transsmalldis;
                m_motion->changeTargetPosition(m_device->axisInfo().transport, pos);
            }
            if (rightIsPress)
            {
                double pos;
                m_motion->getTargetPosition(m_device->axisInfo().transport, pos);
                //                pos += MOVE_DIS;
                pos += m_device->axisInfo().transsmalldis;
                m_motion->changeTargetPosition(m_device->axisInfo().transport, pos);
            }
            if (frontIsPress)
            {
                double pos;
                m_motion->getTargetPosition(m_device->axisInfo().railwidth, pos);
                pos -= m_device->axisInfo().railsmalldis;
                m_motion->changeTargetPosition(m_device->axisInfo().railwidth, pos);
            }
            if (backIsPress)
            {
                double pos;
                m_motion->getTargetPosition(m_device->axisInfo().railwidth, pos);
                pos += m_device->axisInfo().railsmalldis;
                m_motion->changeTargetPosition(m_device->axisInfo().railwidth, pos);
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

void TransportSet::initTransParam()
{
    //获取参数
    m_loadPos.layer        = m_store->intValue("ConfigMagazine", "LoadBoardLayer", 20);
    m_loadPos.firstpos[0]  = m_store->doubleValue("ConfigMagazine", "LoadFirUpPos", 128.254);
    m_loadPos.firstpos[1]  = m_store->doubleValue("ConfigMagazine", "LoadFirDownPos", 14.900);
    m_loadPos.secondpos[0] = m_store->doubleValue("ConfigMagazine", "LoadSecUpPos", 378.431);
    m_loadPos.secondpos[1] = m_store->doubleValue("ConfigMagazine", "LoadSecDownPos", 265.077);
    m_loadPos.thirdpos[0]  = m_store->doubleValue("ConfigMagazine", "LoadThiUpPos", 628.610);
    m_loadPos.thirdpos[1]  = m_store->doubleValue("ConfigMagazine", "LoadThiDownPos", 515.256);
    emit updateMagazinePos(0, m_loadPos);

    m_unloadPos.layer        = m_store->intValue("ConfigMagazine", "UnloadBoardLayer", 20);
    m_unloadPos.firstpos[0]  = m_store->doubleValue("ConfigMagazine", "UnloadFirUpPos", 132.565);
    m_unloadPos.firstpos[1]  = m_store->doubleValue("ConfigMagazine", "UnloadFirDownPos", 19.211);
    m_unloadPos.secondpos[0] = m_store->doubleValue("ConfigMagazine", "UnloadSecUpPos", 383.000);
    m_unloadPos.secondpos[1] = m_store->doubleValue("ConfigMagazine", "UnloadSecDownPos", 269.646);
    m_unloadPos.thirdpos[0]  = m_store->doubleValue("ConfigMagazine", "UnloadThiUpPos", 631.514);
    m_unloadPos.thirdpos[1]  = m_store->doubleValue("ConfigMagazine", "UnloadThiDownPos", 518.160);
    emit updateMagazinePos(1, m_unloadPos);

    //更新界面
    ui->lineEditLoadBoardLayer->setText(QString::number(m_loadPos.layer));
    ui->lineEditLoadFirstUpPos->setText(QString::number(m_loadPos.firstpos[0], 'f', 3));
    ui->lineEditLoadFirstDownPos->setText(QString::number(m_loadPos.firstpos[1], 'f', 3));
    ui->lineEditLoadSecondUpPos->setText(QString::number(m_loadPos.secondpos[0], 'f', 3));
    ui->lineEditLoadSecondDownPos->setText(QString::number(m_loadPos.secondpos[1], 'f', 3));
    ui->lineEditLoadThirdUpPos->setText(QString::number(m_loadPos.thirdpos[0], 'f', 3));
    ui->lineEditLoadThirdDownPos->setText(QString::number(m_loadPos.thirdpos[1], 'f', 3));
    ui->lineEditUnloadBoardLayer->setText(QString::number(m_unloadPos.layer));
    ui->lineEditUnloadFirstUpPos->setText(QString::number(m_unloadPos.firstpos[0], 'f', 3));
    ui->lineEditUnloadFirstDownPos->setText(QString::number(m_unloadPos.firstpos[1], 'f', 3));
    ui->lineEditUnloadSecondUpPos->setText(QString::number(m_loadPos.secondpos[0], 'f', 3));
    ui->lineEditUnloadSecondDownPos->setText(QString::number(m_loadPos.secondpos[1], 'f', 3));
    ui->lineEditUnloadThirdUpPos->setText(QString::number(m_unloadPos.thirdpos[0], 'f', 3));
    ui->lineEditUnloadThirdDownPos->setText(QString::number(m_unloadPos.thirdpos[1], 'f', 3));
    /*
    //获取参数
    int ilbl      = m_store->intValue("ConfigMagazine", "LoadBoardLayer", 20);
    double dblfup = m_store->doubleValue("ConfigMagazine", "LoadFirUpPos", 128.254);
    double dblfdp = m_store->doubleValue("ConfigMagazine", "LoadFirDownPos", 14.900);
    double dblsup = m_store->doubleValue("ConfigMagazine", "LoadSecUpPos", 378.431);
    double dblsdp = m_store->doubleValue("ConfigMagazine", "LoadSecDownPos", 265.077);
    double dbltup = m_store->doubleValue("ConfigMagazine", "LoadThiUpPos", 628.610);
    double dbltdp = m_store->doubleValue("ConfigMagazine", "LoadThiDownPos", 515.256);
    int iubl      = m_store->intValue("ConfigMagazine", "UnloadBoardLayer", 20);
    double dbufup = m_store->doubleValue("ConfigMagazine", "UnloadFirUpPos", 132.565);
    double dbufdp = m_store->doubleValue("ConfigMagazine", "UnloadFirDownPos", 19.211);
    double dbusup = m_store->doubleValue("ConfigMagazine", "UnloadSecUpPos", 383.000);
    double dbusdp = m_store->doubleValue("ConfigMagazine", "UnloadSecDownPos", 269.646);
    double dbutup = m_store->doubleValue("ConfigMagazine", "UnloadThiUpPos", 631.514);
    double dbutdp = m_store->doubleValue("ConfigMagazine", "UnloadThiDownPos", 518.160);

    //更新界面
    ui->lineEditLoadBoardLayer->setText(QString::number(ilbl));
    ui->lineEditLoadFirstUpPos->setText(QString::number(dblfup, 'f', 3));
    ui->lineEditLoadFirstDownPos->setText(QString::number(dblfdp, 'f', 3));
    ui->lineEditLoadSecondUpPos->setText(QString::number(dblsup, 'f', 3));
    ui->lineEditLoadSecondDownPos->setText(QString::number(dblsdp, 'f', 3));
    ui->lineEditLoadThirdUpPos->setText(QString::number(dbltup, 'f', 3));
    ui->lineEditLoadThirdDownPos->setText(QString::number(dbltdp, 'f', 3));
    ui->lineEditUnloadBoardLayer->setText(QString::number(iubl));
    ui->lineEditUnloadFirstUpPos->setText(QString::number(dbufup, 'f', 3));
    ui->lineEditUnloadFirstDownPos->setText(QString::number(dbufdp, 'f', 3));
    ui->lineEditUnloadSecondUpPos->setText(QString::number(dbusup, 'f', 3));
    ui->lineEditUnloadSecondDownPos->setText(QString::number(dbusdp, 'f', 3));
    ui->lineEditUnloadThirdUpPos->setText(QString::number(dbutup, 'f', 3));
    ui->lineEditUnloadThirdDownPos->setText(QString::number(dbutdp, 'f', 3));*/
}

void TransportSet::initPushButton()
{
    //上料移动组
    QButtonGroup *loadMoveBtnGroup = new QButtonGroup;
    loadMoveBtnGroup->addButton(ui->psBtnLoadFirUpMove);
    loadMoveBtnGroup->addButton(ui->psBtnLoadFirDownMove);
    loadMoveBtnGroup->addButton(ui->psBtnLoadSecUpMove);
    loadMoveBtnGroup->addButton(ui->psBtnLoadSecDownMove);
    loadMoveBtnGroup->addButton(ui->psBtnLoadThiUpMove);
    loadMoveBtnGroup->addButton(ui->psBtnLoadThiDownMove);
    connect(loadMoveBtnGroup,
        SIGNAL(buttonClicked(QAbstractButton *)),
        this,
        SLOT(slot_loadMovePos(QAbstractButton *)));

    //上料拾取组
    QButtonGroup *loadPickBtnGroup = new QButtonGroup;
    loadPickBtnGroup->addButton(ui->psBtnLoadFirUpPick);
    loadPickBtnGroup->addButton(ui->psBtnLoadFirDownPick);
    loadPickBtnGroup->addButton(ui->psBtnLoadSecUpPick);
    loadPickBtnGroup->addButton(ui->psBtnLoadSecDownPick);
    loadPickBtnGroup->addButton(ui->psBtnLoadThiUpPick);
    loadPickBtnGroup->addButton(ui->psBtnLoadThiDownPick);
    connect(loadPickBtnGroup,
        SIGNAL(buttonClicked(QAbstractButton *)),
        this,
        SLOT(slot_loadPickPos(QAbstractButton *)));

    //下料移动组
    QButtonGroup *unloadMoveBtnGroup = new QButtonGroup;
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadFirUpMove);
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadFirDownMove);
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadSecUpMove);
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadSecDownMove);
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadThiUpMove);
    unloadMoveBtnGroup->addButton(ui->psBtnUnloadThiDownMove);
    connect(unloadMoveBtnGroup,
        SIGNAL(buttonClicked(QAbstractButton *)),
        this,
        SLOT(slot_unloadMovePos(QAbstractButton *)));

    //下料拾取组
    QButtonGroup *unloadPickBtnGroup = new QButtonGroup;
    unloadPickBtnGroup->addButton(ui->psBtnUnloadFirUpPick);
    unloadPickBtnGroup->addButton(ui->psBtnUnloadFirDownPick);
    unloadPickBtnGroup->addButton(ui->psBtnUnloadSecUpPick);
    unloadPickBtnGroup->addButton(ui->psBtnUnloadSecDownPick);
    unloadPickBtnGroup->addButton(ui->psBtnUnloadThiUpPick);
    unloadPickBtnGroup->addButton(ui->psBtnUnloadThiDownPick);
    connect(unloadPickBtnGroup,
        SIGNAL(buttonClicked(QAbstractButton *)),
        this,
        SLOT(slot_unloadPickPos(QAbstractButton *)));
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
        ui->pushButtonWaitResist->setText("收回待料阻挡");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT7, 0);
        ui->pushButtonWaitResist->setText("升起待料阻挡");
    }
}

void TransportSet::on_pushButtonArriveResist_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT8, 1);
        ui->pushButtonArriveResist->setText("收回到位阻挡");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT8, 0);
        ui->pushButtonArriveResist->setText("升起到位阻挡");
    }
}

void TransportSet::on_pushButtonMaterBoard_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT9, 1);
        ui->pushButtonMaterBoard->setText("收回物料顶板");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT9, 0);
        ui->pushButtonMaterBoard->setText("升起物料顶板");
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
        ui->pushButtonUnloadingPush->setText("推出下料料夹");
    }
}

void TransportSet::on_pushButtonUnloadingBoard_toggled(bool checked)
{
    if (checked)
    {
        m_motion->writeOutBit(DeviceManager::OUT13, 1);
        ui->pushButtonUnloadingBoard->setText("收回下料夹顶升");
    }
    else
    {
        m_motion->writeOutBit(DeviceManager::OUT13, 0);
        ui->pushButtonUnloadingBoard->setText("升起下料夹顶升");
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

void TransportSet::slot_timeout()
{
    if (m_motion->isAvailable())
    {
        double pos = 0.0;
        m_motion->getPosition(m_device->axisInfo().railwidth, pos);
        ui->lineEditRailCurPos->setText(QString::number(pos));
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadCurPos->setText(QString::number(pos));
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadCurPos->setText(QString::number(pos));
    }
}

void TransportSet::slot_buttonPress(QAbstractButton *btn)
{
    ArrowsButton *button = qobject_cast<ArrowsButton *>(btn);
    if (button == ui->pushButtonLeft) { leftIsPress = true; }
    else if (button == ui->pushButtonRight)
    {
        rightIsPress = true;
    }
    else if (button == ui->pushButtonFront)
    {
        frontIsPress = true;
    }
    else if (button == ui->pushButtonBack)
    {
        backIsPress = true;
    }
    m_timeId = startTimer(INTERVAL);
}

void TransportSet::slot_buttonRelease(QAbstractButton *btn)
{
    ArrowsButton *button = qobject_cast<ArrowsButton *>(btn);
    if (button == ui->pushButtonLeft) { leftIsPress = false; }
    else if (button == ui->pushButtonRight)
    {
        rightIsPress = false;
    }
    else if (button == ui->pushButtonFront)
    {
        frontIsPress = false;
    }
    else if (button == ui->pushButtonBack)
    {
        backIsPress = false;
    }
    killTimer(m_timeId);
    m_timeId = 0;
    if (m_motion->isAvailable())
    {
        if (button == ui->pushButtonLeft || button == ui->pushButtonRight)
            m_motion->stop(m_device->axisInfo().transport, CMotionCtrlDev::STOP_IMMEDIATE);
        if (button == ui->pushButtonFront || button == ui->pushButtonBack)
            m_motion->stop(m_device->axisInfo().railwidth, CMotionCtrlDev::STOP_IMMEDIATE);
    }
}

// void TransportSet::slot_leftTimeout()
//{
//    if (m_motion->isAvailable())
//    { m_motion->move(m_device->axisInfo().transport, -0.5, CMotionCtrlDev::POS_RELATIVE); }
//}

// void TransportSet::slot_rightTimeout()
//{
//    if (m_motion->isAvailable())
//    { m_motion->move(m_device->axisInfo().transport, 0.5, CMotionCtrlDev::POS_RELATIVE); }
//}

//    void TransportSet::slot_frontTimeout()
//    {
//        if (m_motion->isAvailable())
//        { m_motion->move(m_device->axisInfo().railwidth, -0.5, CMotionCtrlDev::POS_RELATIVE);
//        }
//    }

//    void TransportSet::slot_backTimeout()
//    {
//        if (m_motion->isAvailable())
//        { m_motion->move(m_device->axisInfo().railwidth, 0.5, CMotionCtrlDev::POS_RELATIVE); }
//    }

void TransportSet::slot_loadMovePos(QAbstractButton *button)
{
    //检测上料夹推出，不能移动
    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN15, value);
    if (value == 1) return;

    double pos       = 0.00;
    QPushButton *btn = qobject_cast<QPushButton *>(button);

    if (btn == ui->psBtnLoadFirUpMove) { pos = ui->lineEditLoadFirstUpPos->text().toDouble(); }
    else if (btn == ui->psBtnLoadFirDownMove)
    {
        pos = ui->lineEditLoadFirstDownPos->text().toDouble();
        qDebug() << "pos" << pos;
    }
    else if (btn == ui->psBtnLoadSecUpMove)
    {
        pos = ui->lineEditLoadSecondUpPos->text().toDouble();
    }
    else if (btn == ui->psBtnLoadSecDownMove)
    {
        pos = ui->lineEditLoadSecondDownPos->text().toDouble();
    }
    else if (btn == ui->psBtnLoadThiUpMove)
    {
        pos = ui->lineEditLoadThirdUpPos->text().toDouble();
    }
    else if (btn == ui->psBtnLoadThiDownMove)
    {
        pos = ui->lineEditLoadThirdDownPos->text().toDouble();
    }
    m_motion->move(m_device->axisInfo().loading, pos, CMotionCtrlDev::POS_ABSOLUTE);
}

void TransportSet::slot_loadPickPos(QAbstractButton *button)
{
    double pos       = 0.00;
    QPushButton *btn = qobject_cast<QPushButton *>(button);

    if (btn == ui->psBtnLoadFirUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadFirstUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnLoadFirDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadFirstDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnLoadSecUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadSecondUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnLoadSecDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadSecondDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnLoadThiUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadThirdUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnLoadThiDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().loading, pos);
        ui->lineEditLoadThirdDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
}

void TransportSet::slot_unloadMovePos(QAbstractButton *button)
{
    //检测下料夹推出，不能移动
    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN39, value);
    if (value == 1) return;

    double pos       = 0.00;
    QPushButton *btn = qobject_cast<QPushButton *>(button);

    if (btn == ui->psBtnUnloadFirUpMove) { pos = ui->lineEditUnloadFirstUpPos->text().toDouble(); }
    else if (btn == ui->psBtnUnloadFirDownMove)
    {
        pos = ui->lineEditUnloadFirstDownPos->text().toDouble();
        qDebug() << "pos" << pos;
    }
    else if (btn == ui->psBtnUnloadSecUpMove)
    {
        pos = ui->lineEditUnloadSecondUpPos->text().toDouble();
    }
    else if (btn == ui->psBtnUnloadSecDownMove)
    {
        pos = ui->lineEditUnloadSecondDownPos->text().toDouble();
    }
    else if (btn == ui->psBtnUnloadThiUpMove)
    {
        pos = ui->lineEditUnloadThirdUpPos->text().toDouble();
    }
    else if (btn == ui->psBtnUnloadThiDownMove)
    {
        pos = ui->lineEditUnloadThirdDownPos->text().toDouble();
    }
    m_motion->move(m_device->axisInfo().unloading, pos, CMotionCtrlDev::POS_ABSOLUTE);
}

void TransportSet::slot_unloadPickPos(QAbstractButton *button)
{
    double pos       = 0.00;
    QPushButton *btn = qobject_cast<QPushButton *>(button);

    if (btn == ui->psBtnUnloadFirUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadFirstUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnUnloadFirDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadFirstDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnUnloadSecUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadSecondUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnUnloadSecDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadSecondDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnUnloadThiUpPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadThirdUpPos->setText(QString::asprintf("%0.3lf", pos));
    }
    else if (btn == ui->psBtnUnloadThiDownPick)
    {
        m_motion->getPosition(m_device->axisInfo().unloading, pos);
        ui->lineEditUnloadThirdDownPos->setText(QString::asprintf("%0.3lf", pos));
    }
}

void TransportSet::on_pushButtonLoading_clicked()
{
    //    emit startLoading();
    bool haveBoard = false;
    emit detectBoard(&haveBoard);
    QThread::msleep(50);
    if (haveBoard)
    {
        QMessageBox::StandardButton result = QMessageBox::information(this,
            "温馨提示",
            "检测到当前检测位有板，是否从此板开始?",
            QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes)
        {
            qDebug() << "从当前板开始检测";
            emit startRun(AutoTransport::detect);
        }
        else
        {
            qDebug() << "当前板不检测";
            emit startRun(AutoTransport::noDetect);
        }
    }
    else
        emit startRun(AutoTransport::noBoard);
}

void TransportSet::on_pushButtonUnloading_clicked()
{
    emit endDetect();
}

void TransportSet::on_pushButtonLoadSave_clicked()
{
    QString strlbl  = ui->lineEditLoadBoardLayer->text();
    QString strlfup = ui->lineEditLoadFirstUpPos->text();
    QString strlfdp = ui->lineEditLoadFirstDownPos->text();
    QString strlsup = ui->lineEditLoadSecondUpPos->text();
    QString strlsdp = ui->lineEditLoadSecondDownPos->text();
    QString strltup = ui->lineEditLoadThirdUpPos->text();
    QString strltdp = ui->lineEditLoadThirdDownPos->text();

    if (strlbl.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入上料板层数");
        return;
    }
    if (strlfup.isEmpty() || strlfdp.isEmpty() || strlsup.isEmpty() || strlsdp.isEmpty() ||
        strltup.isEmpty() || strltdp.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入料盒位置");
        return;
    }
    m_loadPos.layer = strlbl.toInt();
    m_store->cfgSetValue("ConfigMagazine", "LoadBoardLayer", m_loadPos.layer);
    m_loadPos.firstpos[0] = strlfup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadFirUpPos", m_loadPos.firstpos[0]);
    m_loadPos.firstpos[1] = strlfdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadFirDownPos", m_loadPos.firstpos[1]);
    m_loadPos.secondpos[0] = strlsup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadSecUpPos", m_loadPos.secondpos[0]);
    m_loadPos.secondpos[1] = strlsdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadSecDownPos", m_loadPos.secondpos[1]);
    m_loadPos.thirdpos[0] = strltup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadThiUpPos", m_loadPos.thirdpos[0]);
    m_loadPos.thirdpos[1] = strltdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadThiDownPos", m_loadPos.thirdpos[1]);
    /*int ilbl = strlbl.toInt();
    m_store->cfgSetValue("ConfigMagazine", "LoadBoardLayer", ilbl);
    double dblfup = strlfup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadFirUpPos", dblfup);
    double dblfdp = strlfdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadFirDownPos", dblfdp);
    double dblsup = strlsup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadSecUpPos", dblsup);
    double dblsdp = strlsdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadSecDownPos", dblsdp);
    double dbltup = strltup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadThiUpPos", dbltup);
    double dbltdp = strltdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "LoadThiDownPos", dbltdp);*/
    m_store->cfgWriteJson();
    updateMagazinePos(0, m_loadPos);
}

void TransportSet::on_pushButtonUnloadSave_clicked()
{
    QString strubl  = ui->lineEditUnloadBoardLayer->text();
    QString strufup = ui->lineEditUnloadFirstUpPos->text();
    QString strufdp = ui->lineEditUnloadFirstDownPos->text();
    QString strusup = ui->lineEditUnloadSecondUpPos->text();
    QString strusdp = ui->lineEditUnloadSecondDownPos->text();
    QString strutup = ui->lineEditUnloadThirdUpPos->text();
    QString strutdp = ui->lineEditUnloadThirdDownPos->text();

    if (strubl.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入下料板层数");
        return;
    }
    if (strufup.isEmpty() || strufdp.isEmpty() || strusup.isEmpty() || strusdp.isEmpty() ||
        strutup.isEmpty() || strutdp.isEmpty())
    {
        QMessageBox::warning(this, "警告", "请输入料盒位置");
        return;
    }
    m_unloadPos.layer = strubl.toInt();
    m_store->cfgSetValue("ConfigMagazine", "UnloadBoardLayer", m_unloadPos.layer);
    m_unloadPos.firstpos[0] = strufup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadFirUpPos", m_unloadPos.firstpos[0]);
    m_unloadPos.firstpos[1] = strufdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadFirDownPos", m_unloadPos.firstpos[1]);
    m_unloadPos.secondpos[0] = strusup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnLoadSecUpPos", m_unloadPos.secondpos[0]);
    m_unloadPos.secondpos[1] = strusdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnLoadSecDownPos", m_unloadPos.secondpos[1]);
    m_unloadPos.thirdpos[0] = strutup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnLoadThiUpPos", m_unloadPos.thirdpos[0]);
    m_unloadPos.thirdpos[1] = strutdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnLoadThiDownPos", m_unloadPos.thirdpos[1]);
    /*
    int iubl = strubl.toInt();
    m_store->cfgSetValue("ConfigMagazine", "UnloadBoardLayer", iubl);
    double dbufup = strufup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadFirUpPos", dbufup);
    double dbufdp = strufdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadFirDownPos", dbufdp);
    double dbusup = strusup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadSecUpPos", dbusup);
    double dbusdp = strusdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadSecDownPos", dbusdp);
    double dbutup = strutup.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadThiUpPos", dbutup);
    double dbutdp = strutdp.toDouble();
    m_store->cfgSetValue("ConfigMagazine", "UnloadThiDownPos", dbutdp);*/
    m_store->cfgWriteJson();
    updateMagazinePos(1, m_unloadPos);
}
