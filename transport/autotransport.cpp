#include "autotransport.h"

#include <QDebug>
#include <QThread>
#include <QTime>
#include <QTimer>

#define LAYER_NUM 20

// int AutoTransport::m_loadBoardLayer = 0;

AutoTransport::AutoTransport(const DeviceManager *device, QObject *parent)
    : QObject(parent), m_device(device)
{
    m_thread = new QThread;
    this->moveToThread(m_thread);

    m_motion      = m_device->motion();
    m_transptAxis = m_device->axisInfo().transport;
    m_loadAxis    = m_device->axisInfo().loading;
    m_unloadAxis  = m_device->axisInfo().unloading;

    m_thread->start();
    m_motion->writeOutBit(DeviceManager::OUT6, 1);

    initMagazinegPos();

    m_timeout = false;
    m_timer   = new QTimer();
    m_timer->setInterval(2000);
    m_timer->moveToThread(m_thread);
    connect(m_timer, &QTimer::timeout, this, &AutoTransport::slot_timeout);

    m_loadIsRun   = true;
    m_unloadIsRun = true;
    //    m_waitPushIn     = false;
    m_loadCurLayer   = 0;
    m_unloadCurLayer = 2;
}

void AutoTransport::slot_loading()
{
    m_vctLoadFlag.clear();
    if (m_motion && m_motion->isAvailable())
    {
        uint32 value = 0;
        m_motion->readInBit(DeviceManager::IN10, value); //检测到位阻挡原位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT8, 1); //升起到位阻挡
        }
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
            qDebug() << "上料层数：" << m_loadCurLayer << "位置："
                     << m_loadPos.value(m_loadCurLayer) << "上料盒感应信号:" << value;
            m_vctLoadFlag.push_back(value);
            if (m_vctLoadFlag.size() == 3)
            {
                if (!m_vctLoadFlag.value(0) && !m_vctLoadFlag.value(1) && !m_vctLoadFlag.value(2))
                {
                    //三个上料盒都找不到
                    alarm();
                    emit noLoadMagazine();
                    m_loadIsRun = false;
                }
                m_vctLoadFlag.resize(0);
            }

            if (value)
            {
                m_loadBoardLayer = 0;
                bool result      = pushInPlatform(true); //推到顶板
                while (!result)
                {
                    result = pushInPlatform();
                    if (m_loadBoardLayer == 20)
                    {
                        qDebug() << "板层到达20层";
                        break;
                    }
                }
            }
            ++m_loadCurLayer;
            if (m_loadCurLayer == 3) m_loadCurLayer = 0;
        }
    }
}

/*
void AutoTransport::run()
{
    if (m_motion && m_motion->isAvailable())
    {
        uint32 value = 0;
        m_motion->readInBit(DeviceManager::IN15, value); //检测上料匣是否在上位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
        }
        value = 0;
        m_motion->readInBit(DeviceManager::IN10, value); //检测到位阻挡原位
        if (value)
        {
            m_motion->writeOutBit(DeviceManager::OUT8, 1); //升起到位阻挡
        }

#if 0
        for (int i = 0; i < 3; i++)
        {
            if (i == 0 || i == 1) continue;
            //            uint32 value = 0;
            qDebug() << "m_loadPos[" << i << "]=" << m_loadPos[i];
            m_motion->move(m_loadAxis, m_loadPos[i], CMotionCtrlDev::POS_ABSOLUTE);
            while (!m_motion->isIdle(m_loadAxis)) { continue; }
            m_motion->readInBit(DeviceManager::IN2, value);
            qDebug() << "上料匣感应信号" << value;
            if (!value)
                continue;
            else
            {
                moveToLoadBoardPos(i);
            }
        }
#else
        do
        {
            qDebug() << "m_loadCurLayer" << m_loadCurLayer << "m_loadPos"
                     << m_loadPos.value(m_loadCurLayer);
            m_motion->move(
                m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
            while (!m_motion->isIdle(m_loadAxis)) { continue; }
            m_motion->readInBit(DeviceManager::IN2, value);
            qDebug() << "上料匣感应信号" << value;
            if (value) break;
        } while (++m_loadCurLayer < 3);
        if (m_loadCurLayer == 3)
        {
            startAlarm();
            emit noLoadMagazine();
            m_loadCurLayer = 0;
        }
        else
        {
            moveToLoadBoardPos();
        }

#endif
    }
}
*/

void AutoTransport::initMagazinegPos()
{
    m_loadPos[0]   = 0;
    m_loadPos[1]   = 243.112; // 248.112  253.112 258.112
    m_loadPos[2]   = 493.322; // 499.322 505.322
    m_unloadPos[0] = 524.595; //暂时是匣底，是否改为匣顶
    m_unloadPos[1] = 274.820;
    m_unloadPos[2] = 25.225;
    m_dis          = 6.0;
}

void AutoTransport::moveToLoadBoardPos()
{
    //    for (int i = 1; i < LAYER_NUM; i++)
    //    {
    //    m_motion->move(m_loadAxis, m_loadPos[layer] + m_dis * i, CMotionCtrlDev::POS_ABSOLUTE);
    if (m_loadBoardLayer == 20)
    {
        m_loadCurLayer++;
        if (m_loadCurLayer == 3) m_loadCurLayer = 0;
        m_motion->move(m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
        m_loadBoardLayer = 0;
    }
    m_motion->move(m_loadAxis, m_dis, CMotionCtrlDev::POS_RELATIVE);
    m_loadBoardLayer++;
    while (!m_motion->isIdle(m_loadAxis)) { continue; }
    double curPos;
    m_motion->getPosition(m_loadAxis, curPos);
    qDebug() << /* "i=" << i << */ "m_loadAxis cur pos" << curPos;
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
        //             m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
        value = 0;
        m_timer->start();
        while (!m_timeout)
        {
            m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
            if (value)
            {
                qDebug() << "检测到到板信号";
                m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
                m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
                m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                emit startDetect();
                advanceBoard();
                break;
            }
            //            else
            //            {
            //                continue;
            //            }
        }
        m_timer->stop();
    }
    else
    {
        qDebug() << "继续往上层走";
        moveToLoadBoardPos();
    }
    //    }
    //}
}

bool AutoTransport::pushInPlatform(bool firstLayer)
{
    //    for (int i = 1; i < LAYER_NUM; i++)
    //    {
    //    m_motion->move(m_loadAxis, m_loadPos[layer] + m_dis * i, CMotionCtrlDev::POS_ABSOLUTE);
    if (!firstLayer)
    {
        //        if (m_loadBoardLayer == 20)
        //        {
        //            m_loadCurLayer++;
        //            if (m_loadCurLayer == 3) m_loadCurLayer = 0;
        //            m_motion->move(
        //                m_loadAxis, m_loadPos.value(m_loadCurLayer),
        //                CMotionCtrlDev::POS_ABSOLUTE);
        //            m_loadBoardLayer = 0;
        //        }
        m_motion->move(m_loadAxis, m_dis, CMotionCtrlDev::POS_RELATIVE);
        m_loadBoardLayer++;
        while (!m_motion->isIdle(m_loadAxis)) { continue; }
        double curPos;
        m_motion->getPosition(m_loadAxis, curPos);
        qDebug() << /* "i=" << i << */ "m_loadAxis cur pos" << curPos;
    }

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
        //             m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
        value     = 0;
        m_timeout = false;
        m_timer->start();
        while (!m_timeout)
        {
            m_motion->readInBit(DeviceManager::IN6, value); //检测到板信号
            if (value)
            {
                qDebug() << "检测到到板信号";
                m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
                m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
                m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                emit startDetect();
                //                advanceBoard(); //继续上料
                return true;
            }
        }
        m_timer->stop();
        if (m_timeout) //超时
        {
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
                        m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
                        m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
                        m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
                        emit startDetect();
                        //                advanceBoard(); //继续上料
                        return true;
                    }
                }
                m_timer->stop();
                if (m_timeout) //超时
                { return false; }
            }
            else
            {
                qDebug() << "无待板信号，继续往上层走";
            }
        }
    }
    else
    {
        qDebug() << "无进板信号，继续往上层走";
    }
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
        if (m_unloadBoardLayer == 0)
        {
            do
            {
                m_motion->move(m_unloadAxis,
                    m_unloadPos.value(m_unloadCurLayer) + (LAYER_NUM - 1) * m_dis,
                    CMotionCtrlDev::POS_ABSOLUTE);
                m_motion->readInBit(DeviceManager::IN3, value); //检测下料盒信号
                qDebug() << "下料层数：" << m_unloadCurLayer << "位置："
                         << m_unloadPos.value(m_unloadCurLayer) + (LAYER_NUM - 1) * m_dis
                         << "下料盒感应信号:" << value;
                if (value) break;
            } while (++m_unloadCurLayer < 3);
            if (m_unloadCurLayer == 3)
            {
                alarm();
                emit noUnloadMagazine();
                m_unloadCurLayer = 0;
                return;
            }
        }
        else
        {
            m_motion->move(m_unloadAxis, -m_dis, CMotionCtrlDev::POS_RELATIVE);
        }
        while (!m_motion->isIdle(m_unloadAxis)) { continue; }
        double curPos;
        m_motion->getPosition(m_unloadAxis, curPos);
        qDebug() << "m_unloadAxis cur pos" << curPos;

        //        m_vctUnloadFlag.push_back(value);
        //        if (m_vctUnloadFlag.size() == 3)
        //        {
        //            if (!m_vctUnloadFlag.value(0) && !m_vctUnloadFlag.value(1) &&
        //            !m_vctUnloadFlag.value(2))
        //            {
        //                //三个下料盒都找不到
        //                alarm();
        //                emit noUnloadMagazine();
        //                m_unloadIsRun = false;
        //            }
        //            m_vctUnloadFlag.resize(0);
        //        }

        bool result = pushOutPlatform();   //推出顶板
        if (result) result = pressBoard(); //继续压板
        if (result) advanceBoard();        //继续进板

        if (++m_unloadBoardLayer == 20)
        {
            ++m_unloadCurLayer;
            if (m_loadCurLayer == 3) m_loadCurLayer = 0;
            m_unloadBoardLayer = 0;
        }
//    }
#if 1

#else
        for (int i = 0; i < 3; i++)
        {
            if (i == 0 || i == 1) continue;
            qDebug() << "m_unloadPos[" << i << "]=" << m_unloadPos[i];
            m_motion->move(m_unloadAxis,
                m_unloadPos[i] + (LAYER_NUM - 1) * m_dis,
                CMotionCtrlDev::POS_ABSOLUTE);
            while (!m_motion->isIdle(m_unloadAxis)) { continue; }
            m_motion->readInBit(DeviceManager::IN3, value);
            qDebug() << "下料匣感应信号" << value;
            if (!value)
                continue;
            else
            {
                moveToUnloadBoardPos();
            }
        }
#endif
    }
}

void AutoTransport::moveToUnloadBoardPos()
{
    //    for (int i = 1; i < LAYER_NUM; i++)
    //    {
    //        m_motion->move(m_loadAxis, m_loadPos[layer] + m_dis * i,
    //        CMotionCtrlDev::POS_ABSOLUTE);
    m_motion->move(m_unloadAxis, -m_dis, CMotionCtrlDev::POS_RELATIVE);
    while (!m_motion->isIdle(m_unloadAxis)) { continue; }
    double curPos;
    m_motion->getPosition(m_unloadAxis, curPos);
    qDebug() /*<< "i=" << i */ << "m_loadAxis cur pos" << curPos;
    //    QThread::msleep(1000);
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

    value = 0;
    while (1)
    {
        m_motion->readInBit(DeviceManager::IN7, value); //检测出板信号
        if (value)
        {
            qDebug() << "检测到出板信号";
            m_motion->writeOutBit(DeviceManager::OUT8, 1);  //升起到位阻挡
            m_motion->writeOutBit(DeviceManager::OUT13, 1); //抬起下料顶升
            m_motion->writeOutBit(DeviceManager::OUT12, 1); //推出下料夹
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //收回下料夹
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //下料夹顶升下降
            //            m_motion->stop(m_transptAxis,CMotionCtrlDev::STOP_SLOWING);
            emit endDetect();
            pressBoard();
            break;
        }
        else
        {
            continue;
        }
    }
    //    }
}

bool AutoTransport::pushOutPlatform(/*bool firstLayer*/)
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
            m_motion->writeOutBit(DeviceManager::OUT8, 1);  //升起到位阻挡
            m_motion->writeOutBit(DeviceManager::OUT13, 1); //抬起下料顶升
            m_motion->writeOutBit(DeviceManager::OUT12, 1); //推出下料夹
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //收回下料夹
            m_motion->writeOutBit(DeviceManager::OUT12, 0); //下料夹顶升下降
            //            m_motion->stop(m_transptAxis,CMotionCtrlDev::STOP_SLOWING);
            emit endDetect();

            //            break;
            return true;
        }
    }
    m_timer->stop();
    if (m_timeout) qDebug() << "出板错误";
    return false;
}

void AutoTransport::alarm()
{
    for (int i = 0; i < 20; i++)
    {
        m_motion->writeOutBit(DeviceManager::OUT4, 1);
        QThread::msleep(500);
        m_motion->writeOutBit(DeviceManager::OUT4, 0);
        QThread::msleep(500);
    }
}

void AutoTransport::advanceBoard()
{
    if (m_loadBoardLayer == 20)
    {
        m_loadCurLayer++;
        if (m_loadCurLayer == 3) m_loadCurLayer = 0;
        m_motion->move(m_loadAxis, m_loadPos.value(m_loadCurLayer), CMotionCtrlDev::POS_ABSOLUTE);
        m_loadBoardLayer = 0;
    }
    m_motion->move(m_loadAxis, m_dis, CMotionCtrlDev::POS_RELATIVE);
    while (!m_motion->isIdle(m_loadAxis)) { continue; }
    double curPos;
    m_motion->getPosition(m_loadAxis, curPos);
    qDebug() << /* "i=" << i << */ "m_loadAxis cur pos" << curPos;
    uint32 value;
    //    m_motion->readInBit(DeviceManager::IN14, value); //检测上料匣是否在原位
    //    if (value)
    //    {
    qDebug() << "上料匣在原位";
    value = 0;
    m_motion->writeOutBit(DeviceManager::OUT11, 1); //推出上料夹
    QThread::msleep(1000);
    m_motion->writeOutBit(DeviceManager::OUT11, 0); //回收上料夹
    m_motion->readInBit(DeviceManager::IN4, value); //检测进板信号
    if (value)
    {
        qDebug() << "检测到进板信号";
        m_motion->move(m_transptAxis, CMotionCtrlDev::DIR_POSITIVE); //运输轴转动
        value = 0;

        m_timeout = false;
        m_timer->start();
        while (!m_timeout)
        {
            m_motion->readInBit(DeviceManager::IN5, value); //检测待板信号
            if (value)
            {
                qDebug() << "检测检测待板信号";
                m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);

                //                m_waitPushIn = true;
                //                while (m_waitPushIn) { qDebug() << "待料等检测"; }
                break;
            }
        }
        m_timer->stop();
    }
    //    else
    //    {
    //        qDebug() << "未检测到进板信号";
    //        slot_continueLoad();
    //    }
}

bool AutoTransport::pressBoard()
{
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
            m_motion->writeOutBit(DeviceManager::OUT9, 1); //抬顶升
            m_motion->writeOutBit(DeviceManager::OUT7, 1); //升起待料阻挡
            m_motion->stop(m_transptAxis, CMotionCtrlDev::STOP_SLOWING);
            emit startDetect();
            //            advanceBoard();
            //            m_waitPushIn = true;
            //            break;
            return true;
        }
        //        else
        //        {
        //            //            qDebug() << "没检测到到板信号";
        //            continue;
        //        }
    }
    m_timer->stop();
    return false;
}

void AutoTransport::slot_timeout()
{
    m_timeout = true;
    qDebug() << "time out!!";
}
