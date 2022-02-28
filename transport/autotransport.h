#ifndef AUTOTRANSPORT_H
#define AUTOTRANSPORT_H

#include "device/devicemanager.h"

#include <QMap>
#include <QObject>

class AutoTransport : public QObject
{
    Q_OBJECT
public:
    explicit AutoTransport(const DeviceManager *device, QObject *parent = nullptr);

signals:
    void startDetect();
    void endDetect();
    void noLoadMagazine(); //无上料盒

public slots:
    void slot_loading();
    void slot_unloading(); //回收物料

private:
    void initMagazinegPos(); //初始化料盒位置
    void moveToLoadBoardPos();
    void moveToUnloadBoardPos();
    void alarm();

private slots:
    void slot_continueLoad();
    void slot_arriveBoard();

private:
    const DeviceManager *const m_device;
    CMotionCtrlDev *m_motion = nullptr; //运动控制器
    QThread *m_thread;
    bool m_loadIsRun;
    int m_transptAxis;             //运输轴
    int m_loadAxis;                //上料轴
    int m_unloadAxis;              //下料轴
                                   //    double m_loadPos[3];   //上料轴目标位置
                                   //    double m_unloadPos[3]; //下料轴目标位置
    QMap<int, double> m_loadPos;   //上料轴目标位置
    QMap<int, double> m_unloadPos; //下料轴目标位置
    QVector<bool> m_vctLoadFlag;   //料盒标志
    double m_dis;                  //板间距
    int m_loadCurLayer;            //上料当前所在层 从下到上依次为0-1-2
    int m_unloadCurLayer;          //下料当前所在层 从下到上依次为0-1-2
    static int loadBoardLayer;
};

#endif // AUTOTRANSPORT_H
