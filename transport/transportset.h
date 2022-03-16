#ifndef TRANSPORTSET_H
#define TRANSPORTSET_H

#include "autotransport.h"
#include "device/devicemanager.h"

#include <QDialog>

class ArrowsButton;
// class AutoTransport;
class QAbstractButton;

namespace Ui
{
class TransportSet;
}

class TransportSet : public QDialog
{
    Q_OBJECT

public:
    //    struct MagazinePos
    //    {
    //        int layer;
    //        double firstpos[2];
    //        double secondpos[2];
    //        double thirdpos[2];
    //    };

    explicit TransportSet(const DeviceManager *device, QWidget *parent = nullptr);
    ~TransportSet();
    QAction *action() { return m_action; }
    void closeIO();

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private slots:
    void slot_transportAxisMove();
    void slot_railwidthAxisMove();
    void slot_timeout();
    void slot_buttonPress(QAbstractButton *btn);
    void slot_buttonRelease(QAbstractButton *btn);
    //    void slot_leftTimeout();
    //    void slot_rightTimeout();
    void slot_frontTimeout();
    void slot_backTimeout();
    void slot_loadMovePos(QAbstractButton *button);
    void slot_loadPickPos(QAbstractButton *button);
    void slot_unloadMovePos(QAbstractButton *button);
    void slot_unloadPickPos(QAbstractButton *button);

    void on_pushButtonAirSwitch_toggled(bool checked);

    void on_pushButtonWaitResist_toggled(bool checked);

    void on_pushButtonArriveResist_toggled(bool checked);

    void on_pushButtonMaterBoard_toggled(bool checked);

    void on_pushButtonLoadingPush_toggled(bool checked);

    void on_pushButtonUnloadingPush_toggled(bool checked);

    void on_pushButtonUnloadingBoard_toggled(bool checked);

    void on_pushButtonTransport_toggled(bool checked);

    void on_pushButtonLoading_clicked();

    void on_pushButtonUnloading_clicked();

    void on_pushButtonLoadSave_clicked();

    void on_pushButtonUnloadSave_clicked();

private:
    void setButtonStatus(bool flag);
    void initTransParam();
    void initPushButton();

signals:
    void updateMagazinePos(int flag, AutoTransport::MagazinePos param);
    void detectBoard(bool *haveBoard);
    void startRun(int arg);
    void restartLoading();
    void endDetect();

private:
    Ui::TransportSet *ui;
    const DeviceManager *const m_device;
    CStorageInterface *m_store = nullptr; //项目配置存贮接口
    CMotionCtrlDev *m_motion   = nullptr; //运动控制器
    QAction *m_action;                    //启动这个设置页面的action;
    AutoTransport *m_autoTransport;

    ArrowsButton *m_leftButton, *m_rightButton;
    ArrowsButton *m_frontButton, *m_backButton;
    QTimer *timer;

    int m_timeId      = 0; //定时器ID
    bool leftIsPress  = false;
    bool rightIsPress = false;
    AutoTransport::MagazinePos m_loadPos, m_unloadPos;
};

#endif // TRANSPORTSET_H
