#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "acqconfig.h"
#include <QLabel>
#include <QCheckBox>

#include "zauxdll2.h"

#include "imagedeal.h"
#include "sportcontrol.h"

// set code language
#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool LoadGigeCamera(const char *ccfPath);
    bool InitObjects(); //hwnd要显示的设备句柄
    void Snap(); //拍照
    void Grab(); //夺取
    void FreeZe(); //冻结

    Ui::MainWindow *ui;
signals:
    void haveCameraImage(QImage image,ImageDeal::Parameter par,int num);
    void singleSportSignal(void *g_handle,int iaxis,int distance);
    void leftRightOriginSignal(void *g_handle,int imode);
    void autoSportSignal(void *g_handle);
//    void stopAutoSportSignal(void *g_handle);

protected:
    SapAcquisition	*m_Acq;
    SapAcqDevice *m_AcqDevice;
    SapBuffer *m_Buffers;
    SapTransfer *m_Xfer;
    SapView *m_View;

private:

    QString ccfpath;
    QString exepath;
    QString srcpath;
    QString objpath;
    char m_currentConfigDir[MAX_PATH];
    char m_currentConfigFileName[MAX_PATH];
    char acqServerName[CORSERVER_MAX_STRLEN];
    char acqDeviceName[CORSERVER_MAX_STRLEN];
    char configFilename[MAX_PATH];
    int acqDeviceNumber;
    AcqConfig *mAcqConfig;

    static void AcqCallback(SapXferCallbackInfo *pInfo);
    static void XferCallback(SapXferCallbackInfo *pInfo);

    void DestroyObjects();
    void setButtonEnabled(bool flag);
    ZMC_HANDLE g_handle;
    static int num;
    int d_Num;
    ImageDeal *m_ImgDeal; //图像处理类
    QThread *m_ImgDealThread; //图像处理线程
    ImageDeal::Parameter par;

    SportControl *m_SptControl; //运动控制类
    QThread *m_sptContThread; //运动控制线程
    QCheckBox *checkIsTest;
public slots:
    void getCcfFile();
    void SlotSnap();
    void SlotGrab();
    void SlotFreeZe();

    void conController();
    void setLeftShift();
    void setRightShift();
    void setLeftRightOrigin();
    void setClockWise();
    void setAntiClockWise();

    void showResultImage(long time,QImage imgResult,int count);
    void closeImgDealThread();
};

#endif // MAINWINDOW_H
