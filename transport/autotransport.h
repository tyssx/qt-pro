#ifndef AUTOTRANSPORT_H
#define AUTOTRANSPORT_H

#include "device/devicemanager.h"

#include <QMap>
#include <QObject>

class AutoTransport : public QObject
{
    Q_OBJECT
public:
    enum Detect
    {
        noBoard,
        detect,
        noDetect
    };
    explicit AutoTransport(const DeviceManager *device, QObject *parent = nullptr);
    ~AutoTransport();

signals:
    void waitDetect(); //物料放置完毕待检测
    void endDetect();
    void noLoadMagazine();   //无上料盒
    void noUnloadMagazine(); //无下料盒
    void magazineFindError();
    void advanceBoardError(); //进板错误

public slots:
    ///
    /// \brief slot_run 开始运行
    /// \param arg 当前中心顶板是否存在，是否需要检测参数
    ///
    void slot_run(int arg);
    void slot_restartLoading();             //从头开始运行上料
    void slot_unloading();                  //回收物料
    void slot_detectBoard(bool *haveBoard); //检测顶升是否有板
    void slot_toggleBoard(bool flag);       //单独升降板
    void slot_pushOutBoard();               //退板

private:
    void initMagazinegPos();                      //初始化料盒位置
    void pushBoardIntoRail(int location);           //送板进轨道
    bool pushInPlatform(bool firstLayer = false); //推入顶板
    bool pushInWaitSite(bool firstLayer = false); //推入待料处
    bool pushBoardOutoRail();                    //送板出轨道
    bool pushOutPlatform();                       //推出顶板
    bool pressBoard();                            //压板
    bool advanceBoard(bool isCenter);
    void alarm(); //报警

private slots:
    void slot_timeout();

private:
    enum Location
    {
        platform, //顶升
        waitsite  //待料阻挡
    };
    const DeviceManager *const m_device;
    CMotionCtrlDev *m_motion = nullptr; //运动控制器
    QThread *m_thread;
    int m_transptAxis;             //运输轴
    int m_loadAxis;                //上料轴
    int m_unloadAxis;              //下料轴
                                   //    double m_loadPos[3];   //上料轴目标位置
                                   //    double m_unloadPos[3]; //下料轴目标位置
    QMap<int, double> m_loadPos;   //上料轴目标位置
    QMap<int, double> m_unloadPos; //下料轴目标位置
    QVector<bool> m_vctLoadFlag, m_vctUnloadFlag; //上料盒标志
    double m_dis;                                 //板间距
    int m_loadCurLayer;                           //上料当前所在层 从下到上依次为0-1-2
    int m_unloadCurLayer;                         //下料当前所在层 从下到上依次为0-1-2
    int m_loadBoardLayer;                         //上料板层数
    int m_unloadBoardLayer;                       //下料板层数
    QTimer *m_timer;
    bool m_timeout;
    int m_num;
};

#endif // AUTOTRANSPORT_H
