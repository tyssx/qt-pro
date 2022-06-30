#ifndef CAMLIGHTDLG_H
#define CAMLIGHTDLG_H

#include "CStorageInterface.h"
#include "esCameraLib/esCamera.h"
#include "esCameraLib/esCameraManage.h"
#include "esviRConn/RobotFactory.h"

#include <QWidget>
namespace Ui
{
class camLightDlg;
}

class camLightDlg : public QWidget
{
    Q_OBJECT

public:
    explicit camLightDlg(QWidget *parent = nullptr);
    ~camLightDlg();
    void initInfo();
    void setStoreCtrl(CStorageInterface *store) { pStore = store; }
    void setQSettingsCtrl(QSettings *setting) { m_settings = setting; }
    void closeDevice();
    void getLightValue(int &lWhite, int &lRed, int &lGreen, int &lBlue);
    void resetLight();

public slots:
    void on_continuousmodel_clicked();
    void slotSetAllLightValue(int lWhite, int lRed, int lGreen, int lBlue, int count);

private slots:
    void setLightPort(const QString &arg1);

    void on_horizontalSlider_LightValue_valueChanged(int value);

    void on_spinBox_LightValue_valueChanged(int arg1);

    void on_horizontalSlider_LightValue1_valueChanged(int value);

    void on_spinBox_LightValue1_valueChanged(int arg1);

    void on_horizontalSlider_LightValue2_valueChanged(int value);

    void on_spinBox_LightValue2_valueChanged(int arg1);

    void on_horizontalSlider_LightValue3_valueChanged(int value);

    void on_spinBox_LightValue3_valueChanged(int arg1);

    void on_singlemodel_clicked();

    //    void on_continuousmodel_clicked();

    void on_externalmodel_clicked();

    void on_expouseslider_valueChanged(int value);

    void on_expousebox_editingFinished();

    void on_gainslider_valueChanged(int value);

    void on_gainbox_editingFinished();

private:
    Ui::camLightDlg *ui;
    struct lightInfo
    {
        QString port;
        int red;
        int green;
        int blue;
        int white;
        void init() { red = green = blue = white = 0; }
    };

    RobotConnet *m_light        = nullptr;
    esCamera *pCamera           = nullptr;
    CStorageInterface *pStore   = nullptr;
    QSettings *m_settings       = nullptr;
    esCameraManage *esCamPlugin = nullptr;
    bool isContinus             = false;
    lightInfo m_lightInfo;
    QString cameraSerial;
    QString cameraModel;

private:
    void initForm();
    void initLightInfo();
    void openLight();
    void closeLight();
    void getCameraInfoValue();
    void setLightValue(int port, int value);
    void initCameraPlugin(const QString &filePath, const QString &fileName);
};

#endif // CAMLIGHTDLG_H
