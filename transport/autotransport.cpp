#include "autotransport.h"

#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>

#define LAYER_NUM 20 //料盒装板层数

AutoTransport::AutoTransport(const DeviceManager *device, QObject *parent)
    : QObject(parent), m_device(device)
{
    m_thread = new QThread;
    this->moveToThread(m_thread);
    m_thread->start();

    m_loadIsRun = true;
    //    m_unloadIsRun = true;
    //    m_waitPushIn     = false;
    m_loadCurLayer     = 0;
    m_loadBoardLayer   = 0;
    m_unloadCurLayer   = 2;
    m_unloadBoardLayer = 0;
    m_num              = 0;

    m_motion      = m_device->motion();
    m_transptAxis = m_device->axisInfo().transport;
    m_loadAxis    = m_device->axisInfo().loading;
    m_unloadAxis  = m_device->axisInfo().unloading;

    //打开气压，降下顶升阻挡
    m_motion->writeOutBit(DeviceManager::OUT6, 1);
    m_motion->writeOutBit(DeviceManager::OUT7, 0);
    m_motion->writeOutBit(DeviceManager::OUT8, 0);
    m_motion->writeOutBit(DeviceManager::OUT9, 0);

    initMagazinegPos();

    m_timeout = false;
    m_timer   = new QTimer();
    m_timer->moveToThread(m_thread);
    m_timer->setInterval(4000);
    connect(m_timer, &QTimer::timeout, this, &AutoTransport::slot_timeout);
}

AutoTransport::~AutoTransport()
{
    m_thread->exit();
    m_thread->wait();
    delete m_thread;
}

void AutoTransport::slot_run(int arg)
{
    //运行开始
    if (!m_motion || !m_motion->isAvailable()) return;

    m_motion->writeOutBit(DeviceManager::OUT7, 0); //回收待料阻挡
    m_motion->writeOutBit(DeviceManager::OUT8, 0); //回收到位阻挡
    m_motion->writeOutBit(DeviceManager::OUT9, 0); //回收顶升

    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN7, value); //检测出板信号
    if (value)
    {
        qDebug() << "检测到出板信号";
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动
        //        m_motion->writeOutBit(DeviceManager::OUT8, 1);               //升起到位阻挡
        QThread::msleep(300);
        m_motion->writeOutBit(DeviceManager::OUT13, 1); //抬起下料顶升
        m_motion->writeOutBit(DeviceManager::OUT12, 1); //推出下料夹
        QThread::msleep(1000);
        m_motion->writeOutBit(DeviceManager::OUT12, 0); //收回下料夹
        m_motion->writeOutBit(DeviceManager::OUT13, 0); //下料夹顶升下降
        m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
    }

    switch (arg)
    {
    case noBoard:
    {
        m_motion->readInBit(DeviceManager::IN10, value); //检测到位阻挡原位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT8, 1); //升起到位阻挡
        }
        m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
        if (value)
        {
            qDebug() << "检测有待板";
            bool isPress = pressBoard(); //待料压板
            qDebug() << "isPress 待料压板" << isPress;
            firstAdvance();
        }
        else
        {
            slot_restartLoading();
        }
    }
    break;
    case detect:
    {
        m_motion->writeOutBit(DeviceManager::OUT7, 1);  //升起待料阻挡
        m_motion->writeOutBit(DeviceManager::OUT8, 1);  //升起到位阻挡
        m_motion->writeOutBit(DeviceManager::OUT9, 1);  //抬顶升
        m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
        if (!value)
        {
            firstAdvance(); //进板
        }
    }
    break;
    case noDetect:
    {
        m_motion->readInBit(DeviceManager::IN8, value); //检测待料阻挡原位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
        }
        QThread::msleep(200);
        bool result = pushOutPlatform(); //推出顶板
        qDebug() << "result 推出顶板" << result;
        if (result)
        {
            m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
            if (value)
            {
                qDebug() << "检测有待板";
                bool isPress = pressBoard(); //待料压板
                qDebug() << "isPress 待料压板" << isPress;
                firstAdvance();
                /*bool result =
                    advanceBoard(!isPress); //继续进板 isPress false进到到位感应 true进到待位感应
                ++m_loadBoardLayer;
                while (!result)
                {
                    result = advanceBoard(!isPress);
                    ++m_loadBoardLayer;
                    if (m_num == 3)
                    {
                        emit advanceBoardError();
                        qDebug() << "emit advanceBoardError";
                        alarm();
                        m_num = 0;
                        break;
                    }
                }*/
            }
            else
            {
                slot_restartLoading();
            }
        }
    }
    break;
    default: break;
    }
}

void AutoTransport::slot_restartLoading()
{
    //首次上料
    m_vctLoadFlag.clear();
    m_loadIsRun = true;
    if (m_motion && m_motion->isAvailable())
    {
        uint32 value = 0;
        m_motion->readInBit(DeviceManager::IN10, value); //检测到位阻挡原位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT8, 1); //升起到位阻挡
        }
        m_motion->readInBit(DeviceManager::IN13, value); //检测顶升上位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT9, 0); //收起顶升
        }
        m_motion->readInBit(DeviceManager::IN9, value); //检测待料阻挡上位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT7, 0); //收起待料阻挡
        }
        qDebug() << "开始首次上料";
        while (m_loadIsRun)
        {
            m_motion->readInBit(DeviceManager::IN15, value); //检测上料匣是否在上位
            if (value)
            {
                m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
            }

            m_motion->move(
                m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
            while (!m_motion->isIdle(m_loadAxis)) { continue; }
            m_motion->readInBit(DeviceManager::IN2, value); //检测上料盒信号
            qDebug() << "首次上料，当前上料盒在" << m_loadCurLayer << "层，位置在："
                     << m_loadPos.value(m_loadCurLayer) << "，上料盒感应信号:" << value;
            m_vctLoadFlag.push_back(value);
            if (m_vctLoadFlag.size() == 3)
            {
                if (!m_vctLoadFlag.value(0) && !m_vctLoadFlag.value(1) && !m_vctLoadFlag.value(2))
                {
                    //三个上料盒都未找到，报警
                    qDebug() << "三个上料盒都未找到";
                    emit noLoadMagazine();
                    alarm();
                    m_loadIsRun = false;
                }
                //                m_vctLoadFlag.resize(0);
            }

            if (value)
            {
                m_loadBoardLayer = 0;
                bool result      = pushInPlatform(true,true); //推到顶板
                while (!result)
                {
                    result = pushInPlatform(true);
                    if (m_loadBoardLayer == LAYER_NUM)
                    {
                        qDebug() << "板层到达20层";
                        m_loadBoardLayer = 0;
                        break;
                    }
                }
                if (result)
                {
                    m_loadIsRun = false;
                    continue;
                }
                else
                {
                    if (m_vctLoadFlag.size() == 3)
                    {
                        qDebug() << "轨道未收到物料";
                        emit magazineFindError();
                        alarm();
                        m_loadIsRun = false;
                    }
                }
            }
            if (m_vctLoadFlag.size() == 3) m_vctLoadFlag.resize(0);
            if (++m_loadCurLayer == 3) m_loadCurLayer = 0;
        }
    }
}

void AutoTransport::initMagazinegPos()
{
    m_loadPos[0]   = 10.805;  // 6.451;
    m_loadPos[1]   = 260.936; // 256.582;//4.354
    m_loadPos[2]   = 510.666; // 506.951; 513.066*// 621.096
    m_unloadPos[0] = 24.22;   // 17.740;  //暂时是匣底，是否改为匣顶
    m_unloadPos[1] = 268.646;
    m_unloadPos[2] = 518.060;
    m_dis          = 6.008;
}

bool AutoTransport::pushInPlatform(bool isCenter,bool firstLayer)
{
    qDebug() << "pushInPlatform 首次推料到顶升-----------"
             << "firstLayer" << firstLayer;
    if (!firstLayer)
    {
        m_motion->move(m_loadAxis, m_dis, CMotionCtrlDev::POS_RELATIVE);
        while (!m_motion->isIdle(m_loadAxis)) { continue; }
        double curPos;
        m_motion->getPosition(m_loadAxis, curPos);
        qDebug() << "上料轴当前位置：" << curPos;
    }
    //    m_loadBoardLayer++;

    //    QThread::msleep(1000);
    uint32 value = 0;
    //    m_motion->readInBit(DeviceManager::IN14, value); //检测上料匣是否在原位
    //    if (value)
    //    {
    //    qDebug() << "上料匣在原位";
    m_motion->writeOutBit(DeviceManager::OUT11, 1); //推出上料夹
    QThread::msleep(1000);
    m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
    m_motion->readInBit(DeviceManager::IN4, value); //检测进板信号
    if (value)
    {
        qDebug() << "检测到进板信号";
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动
        if(isCenter)  //推到到板位置
        {
            m_timeout = false;
            m_timer->start();
            while (!m_timeout)
            {
                m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
                if (value)
                {
                    qDebug() << "检测到到板信号";
                    QThread::msleep(150);
    //                m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
                    m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
                    m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                    emit waitDetect();
                    m_timer->stop();
                    ++m_loadBoardLayer;  //加一层
                    advanceBoard(false); //继续上料
                    ++m_loadBoardLayer;
                    return true;
                }
                //            QCoreApplication::processEvents(QEventLoop::AllEvents,
                //            100);
            }
            m_timer->stop();
            if (m_timeout) //超时
            {
                qDebug() << "timeout 检测到板信号超时";
                m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
                if (value)
                {
                    qDebug() << "检测到待板信号";
                    m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动
                    value     = 0;
                    m_timeout = false;
                    m_timer->start();
                    while (!m_timeout)
                    {
                        m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
                        if (value)
                        {
                            qDebug() << "检测到到板信号";
                            QThread::msleep(150);
    //                        m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
                            m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
                            m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                            emit waitDetect();
                            ++m_loadBoardLayer;  //加一层
                            advanceBoard(false); //继续上料
                            ++m_loadBoardLayer;
                            return true;
                        }
                        //                    QCoreApplication::processEvents(QEventLoop::AllEvents,
                        //                    100);
                    }
                    m_timer->stop();
                    if (m_timeout) //超时
                    {
                        qDebug() << "timeout 再次检测到板信号超时";
                        //                    return false;
                    }
                }
                else
                {
                    qDebug() << "无待板信号，继续往上层走";
                }
            }
        }
        else  //推到待板位置
        {
            m_timeout = false;
            m_timer->start();
            while (!m_timeout)
            {
                m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
                if(value)
                {
                    qDebug() << "检测到待板信号";
                    QThread::msleep(400); //继续向前走到待位阻挡
                    m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                    m_timer->stop();
                    ++m_loadBoardLayer;  //加一层
                    return true;
                }
            }
            m_timer->stop();
        }

    }
    else
    {
        qDebug() << "无进板信号，继续往上层走";
        QThread::msleep(1000);//延时，防止推料过快
    }
    ++m_loadBoardLayer;
    return false;
}

void AutoTransport::slot_unloading()
{
    //检测完毕一板，发送一个信号
    if (m_motion && m_motion->isAvailable())
    {
        uint32 value = 0;

        //        while (m_unloadIsRun)
        //        {
        m_motion->readInBit(DeviceManager::IN39, value); //检测下料匣是否在上位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //回收下料夹
        }
        m_motion->writeOutBit(DeviceManager::OUT13, 0); //下料夹顶升下降
        qDebug() << "下料后上料-------------------------";
        if (m_unloadBoardLayer == 0)
        {
            int num = 0;
            do
            {
                m_motion->move(m_unloadAxis,
                    m_unloadPos.value(m_unloadCurLayer) + (LAYER_NUM - 1) * m_dis,
                    CMotionCtrlDev::POS_ABSOLUTE);
                while (!m_motion->isIdle(m_unloadAxis)) { continue; }
                m_motion->readInBit(DeviceManager::IN3, value); //检测下料盒信号
                qDebug() << "当前下料盒在" << m_unloadCurLayer << "层，位置在："
                         << m_unloadPos.value(m_unloadCurLayer) + (LAYER_NUM - 1) * m_dis
                         << "，下料盒感应信号:" << value;
                if (value) break;
                if (--m_unloadCurLayer == -1) m_unloadCurLayer = 2;
            } while (++num < 3);
            if (num == 3)
            {
                //三个下料盒都未找到，报警
                qDebug() << "三个下料盒都未找到";
                emit noUnloadMagazine();
                alarm();
                return;
            }
        }
        else
        {
            m_motion->move(m_unloadAxis, -m_dis, CMotionCtrlDev::POS_RELATIVE);
            while (!m_motion->isIdle(m_unloadAxis)) { continue; }
            double curPos;
            m_motion->getPosition(m_unloadAxis, curPos);
            qDebug() << "下料轴当前位置：" << curPos;
        }

        bool result = pushOutPlatform(); //推出顶板
        qDebug() << "result 推出顶板" << result;
        if (result)
        {
            bool isPress = pressBoard(); //待料压板
            qDebug() << "isPress 继续压板" << isPress;
            result = advanceBoard(!isPress); //继续进板 isPress false进到到位感应 true进到待位感应
            ++m_loadBoardLayer;
            while (!result)
            {
                QThread::msleep(1000); //延时，防止推料过快
                result = advanceBoard(!isPress);
                ++m_loadBoardLayer;
                if (m_num == 3)
                {
                    emit advanceBoardError();
                    qDebug() << "emit advanceBoardError";
                    alarm();
                    m_num = 0;
                    break;
                }
            }
        }

        if (++m_unloadBoardLayer == LAYER_NUM)
        {
            --m_unloadCurLayer;
            if (m_unloadCurLayer == -1) m_unloadCurLayer = 2;
            m_unloadBoardLayer = 0;
        }
    }
}

void AutoTransport::slot_detectBoard(bool *haveBoard)
{
    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
    if (value)
    {
        qDebug() << "检测到顶升有板";
        *haveBoard = true;
    }
    else
    {
        qDebug() << "检测到顶升没有板";
        *haveBoard = false;
    }
}

void AutoTransport::slot_toggleBoard(bool flag)
{
    if (m_motion && m_motion->isAvailable())
    {
        if (flag)
        {
            m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
            //            m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
        }
        else
        {
            m_motion->writeOutBit(DeviceManager::OUT9, 0); //抬顶升
            //            m_motion->writeOutBit(DeviceManager::OUT7, 0); //升起待料阻挡
        }
    }
}

void AutoTransport::slot_pushOutBoard()
{
    pushOutPlatform();
}

bool AutoTransport::pushOutPlatform()
{
    //    if (!firstLayer)
    //    {
    //        m_motion->move(m_unloadAxis, -m_dis, CMotionCtrlDev::POS_RELATIVE);
    //        m_unloadBoardLayer++;
    //        while (!m_motion->isIdle(m_unloadAxis)) { continue; }
    //        double curPos;
    //        m_motion->getPosition(m_unloadAxis, curPos);
    //        qDebug() << "m_loadAxis cur pos" << curPos;
    //}
    //    QThread::msleep(1000);
    qDebug() << "pushOutPlatform 开始下料-------------";
    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN13, value); //检测顶升上位
    if (value)
    {
        m_motion->writeOutBit(DeviceManager::OUT9, 0); //回收顶升
    }
    m_motion->readInBit(DeviceManager::IN11, value); //检测到位阻挡上位
    if (value)
    {
        m_motion->writeOutBit(DeviceManager::OUT8, 0); //回收到位阻挡
    }
    qDebug() << "回收顶升、到位阻挡";
    if (m_motion->isIdle(m_transptAxis))
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动

    value     = 0;
    m_timeout = false;
    m_timer->start();
    while (!m_timeout)
    {
        m_motion->readInBit(DeviceManager::IN7, value); //检测出板信号
        if (value)
        {
            qDebug() << "检测到出板信号";
            m_motion->writeOutBit(DeviceManager::OUT8, 1); //升起到位阻挡
            QThread::msleep(300);
            m_motion->writeOutBit(DeviceManager::OUT13, 1); //抬起下料顶升
            m_motion->writeOutBit(DeviceManager::OUT12, 1); //推出下料夹
            QThread::msleep(1000);
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //收回下料夹
            m_motion->writeOutBit(DeviceManager::OUT13, 0); //下料夹顶升下降
            //            m_motion->stop(m_transptAxis,CMotionCtrlDev::STOP_SLOWING);
            emit endDetect();

            //            break;
            m_timer->stop();
            return true;
        }
        //        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    m_timer->stop();
    if (m_timeout) qDebug() << "出板错误";
    return false;
}

void AutoTransport::alarm()
{
    qDebug() << "alarm 报警---------";
    for (int i = 0; i < 20; i++)
    {
        if (i < 3) m_motion->writeOutBit(DeviceManager::OUT5, 1);
        if (i == 3) m_motion->writeOutBit(DeviceManager::OUT5, 0);
        m_motion->writeOutBit(DeviceManager::OUT4, 1);
        QThread::msleep(500);
        m_motion->writeOutBit(DeviceManager::OUT4, 0);
        QThread::msleep(500);
    }
}

bool AutoTransport::advanceBoard(bool isCenter)
{
    qDebug() << "advanceBoard 推料前进----------------";
    uint32 value    = 0;
    bool firstLayer = false;
#if 1
    qDebug() << "料盒层数" << m_loadCurLayer << "板层数" << m_loadBoardLayer;
    if (m_loadBoardLayer == LAYER_NUM)
    {
        int num = 0;
        if (++m_loadCurLayer == 3) m_loadCurLayer = 0;
        m_loadBoardLayer = 0;

        do
        {
            m_motion->move(
                m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
            while (!m_motion->isIdle(m_loadAxis)) { continue; }
            m_motion->readInBit(DeviceManager::IN2, value); //检测上料盒信号
            qDebug() << "当前上料盒在" << m_loadCurLayer << "层，位置在："
                     << m_loadPos.value(m_loadCurLayer) << "，上料盒感应信号:" << value;
            if (value)
            {
                firstLayer = true;
                break;
            }
            qDebug() << "料盒层数1" << m_loadCurLayer;
            if (++m_loadCurLayer == 3) m_loadCurLayer = 0;
            qDebug() << "料盒层数2" << m_loadCurLayer;
            qDebug() << "num" << num;
        } while (++num < 3);
        if (num == 3)
        {
            emit noLoadMagazine();
            alarm();
            return false;
        }
        m_num++;
    }
#else
    if (m_loadBoardLayer == 20)
    {
        m_loadCurLayer++;
        if (m_loadCurLayer == 3) m_loadCurLayer = 0;
        m_motion->move(m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
        m_loadBoardLayer = 0;
    }
#endif
    if (!firstLayer)
    {
        m_motion->move(m_loadAxis, m_dis, CMotionCtrlDev::POS_RELATIVE);
        //        m_loadBoardLayer++;
        while (!m_motion->isIdle(m_loadAxis)) { continue; }
        double curPos;
        m_motion->getPosition(m_loadAxis, curPos);
        qDebug() << "上料轴当前位置：" << curPos;
    }

    //    m_motion->readInBit(DeviceManager::IN14, value); //检测上料匣是否在原位
    //    if (value)
    //    {
    qDebug() << "推出上料夹";
    m_motion->writeOutBit(DeviceManager::OUT11, 1); //推出上料夹
    QThread::msleep(1000);
    m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
    m_motion->readInBit(DeviceManager::IN4, value); //检测进板信号
    if (value)
    {
        qDebug() << "检测到进板信号";
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动

        m_timeout = false;
        m_timer->start();
        while (!m_timeout)
        {
            if (!isCenter)
            {
                m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
                if (value)
                {
                    qDebug() << "检测到待板信号";
                    QThread::msleep(400); //继续向前走到待位阻挡
                    m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);

                    //                m_waitPushIn = true;
                    //                while (m_waitPushIn) { qDebug() << "待料等检测"; }
                    //                    break;
                    m_timer->stop();
                    return true;
                }
                //                QCoreApplication::processEvents(QEventLoop::AllEvents,
                //                100);
            }
            else
            {
                m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
                if (value)
                {
                    qDebug() << "检测到到板信号";
                    QThread::msleep(150);
                    m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                    //                    break;
                    m_timer->stop();
                    return true;
                }
                //                QCoreApplication::processEvents(QEventLoop::AllEvents,
                //                100);
            }
        }
        m_timer->stop();
    }
    //    else
    //    {
    //        qDebug() << "未检测到进板信号";
    //        slot_continueLoad();
    //    }
    return false;
}

void AutoTransport::firstAdvance()
{
    uint32 value = 0;
    bool isRun = true;
    while (isRun)
    {
        m_motion->readInBit(DeviceManager::IN15, value); //检测上料匣是否在上位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
        }
        m_motion->move(
            m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
        while (!m_motion->isIdle(m_loadAxis)) { continue; }
        m_motion->readInBit(DeviceManager::IN2, value); //检测上料盒信号
        qDebug() << "首次上料，当前上料盒在" << m_loadCurLayer << "层，位置在："
                 << m_loadPos.value(m_loadCurLayer) << "，上料盒感应信号:" << value;
        m_vctLoadFlag.push_back(value);
        if (m_vctLoadFlag.size() == 3)
        {
            if (!m_vctLoadFlag.value(0) && !m_vctLoadFlag.value(1) && !m_vctLoadFlag.value(2))
            {
                //三个上料盒都未找到，报警
                qDebug() << "三个上料盒都未找到";
                emit noLoadMagazine();
                alarm();
                isRun = false;
            }
            //                m_vctLoadFlag.resize(0);
        }
        if(value)
        {
            m_loadBoardLayer = 0;
            bool result      = pushInPlatform(false,true); //推到顶板
            while (!result)
            {
                result = pushInPlatform(false);
                if (m_loadBoardLayer == LAYER_NUM)
                {
                    qDebug() << "板层到达20层";
                    m_loadBoardLayer = 0;
                    break;
                }
            }
            if (result)
            {
                isRun = false;
                continue;
            }
            else
            {
                if (m_vctLoadFlag.size() == 3)
                {
                    qDebug() << "轨道未收到物料";
                    emit magazineFindError();
                    alarm();
                    isRun = false;
                }
            }
        }
        if (m_vctLoadFlag.size() == 3) m_vctLoadFlag.resize(0);
        if (++m_loadCurLayer == 3) m_loadCurLayer = 0;
    }
}

bool AutoTransport::pressBoard()
{
    qDebug() << "pressBoard 待料压板-------------";
    uint32 value = 0;
    m_motion->readInBit(DeviceManager::IN9, value); //检测待料阻挡上位
    if (value)
    {
        m_motion->writeOutBit(DeviceManager::OUT7, 0); //回收待料阻挡
    }
    if (m_motion->isIdle(m_transptAxis))
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动

    m_timeout = false;
    m_timer->start();
    while (!m_timeout)
    {
        m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
        if (value)
        {
            qDebug() << "检测到到板信号";
            QThread::msleep(150);
            m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
            m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
            m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
            emit waitDetect();
            //            advanceBoard();
            //            m_waitPushIn = true;
            //            break;
            m_timer->stop();
            return true;
        }
        QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents /*AllEvents*/, 100);
        //        else
        //        {
        //            //            qDebug() << "没检测到到板信号";
        //            continue;
        //        }
    }
    m_timer->stop();
    if (m_timer)
    {
        m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
        qDebug() << "timeout 没有检测到板信号";
    }
    return false;
}

void AutoTransport::slot_timeout()
{
    m_timeout = true;
    qDebug() << "time out!!";
}
