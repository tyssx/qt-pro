#include "camlightdlg.h"

#include "mainwindow.h"
#include "ui_camlightdlg.h"

#include <QDebug>
#include <QMessageBox>

camLightDlg::camLightDlg(QWidget *parent) : QWidget(parent), ui(new Ui::camLightDlg)
{
    ui->setupUi(this);
    //设置窗口形式，dialog形式，打开界面不会显示
    setWindowFlag(Qt::Dialog);
    ui->tabWidget->setCurrentIndex(0);
    m_lightInfo.init();
}

camLightDlg::~camLightDlg()
{
    delete ui;
    delete m_light;
}

void camLightDlg::initForm()
{
    ui->label_20->setVisible(false);
    ui->label_21->setVisible(false);
    ui->label_22->setVisible(false);
    ui->label_23->setVisible(false);
    ui->label_24->setVisible(false);
    ui->lineEdit_padcoef->setVisible(false);
    ui->lineEdit_secNumH->setVisible(false);
    ui->lineEdit_secNumW->setVisible(false);
    ui->lineEdit_targetgray->setVisible(false);
    ui->lineEdit_calibmethod->setVisible(false);
    ui->checkBox_isNeedLSCCorrect->setVisible(false);
    ui->checkBox_rotateImage->setEnabled(false);
    //初始化光源UI
    initLightInfo();
    connect(ui->comboBox_LightPort, &QComboBox::currentTextChanged, this, &camLightDlg::setLightPort);
}

void camLightDlg::initLightInfo()
{
    if (!m_settings) return;
    //获取所有的串口号并分割
    QStringList slist = m_light->getAllPortNameAuto().split(",");
    for (int i = 0; i < slist.size(); ++i)
    {
        //设置获取到的串口号到comboBox
        ui->comboBox_LightPort->addItem(slist[i]);
    }
    //获取光源的参数值
    int lightValue = m_settings->value("/ConfigLight/LightValue").toInt();
    int ch1        = m_settings->value("/ConfigLight/Channel1").toInt();
    int ch2        = m_settings->value("/ConfigLight/Channel2").toInt();
    int ch3        = m_settings->value("/ConfigLight/Channel3").toInt();
    //设置到UI
    ui->comboBox_LightPort->setCurrentText(m_settings->value("/ConfigLight/LightPort").toString());
    ui->horizontalSlider_LightValue->setValue(lightValue);
    ui->horizontalSlider_LightValue1->setValue(ch1);
    ui->horizontalSlider_LightValue2->setValue(ch2);
    ui->horizontalSlider_LightValue3->setValue(ch3);
}
void camLightDlg::openLight()
{
    if (!m_settings || !m_light) return;
    QString lightPort = m_settings->value("/ConfigLight/LightPort").toString();
    if (lightPort.isEmpty())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("串口配置读取失败!"));
        return;
    }
    m_lightInfo.port = lightPort;
    qDebug() << "光源串口是：" << lightPort;
    int lightValue    = m_settings->value("/ConfigLight/LightValue").toInt();
    int ch1           = m_settings->value("/ConfigLight/Channel1").toInt();
    int ch2           = m_settings->value("/ConfigLight/Channel2").toInt();
    int ch3           = m_settings->value("/ConfigLight/Channel3").toInt();
    m_lightInfo.white = lightValue;
    m_lightInfo.red   = ch1;
    m_lightInfo.green = ch2;
    m_lightInfo.blue  = ch3;
    if (m_light->openSerialPort(lightPort, "115200", 3, 0, 0))
    {
        ui->comboBox_LightPort->setCurrentText(lightPort);
        setLightValue(0, lightValue);
        setLightValue(1, ch1);
        setLightValue(2, ch2);
        setLightValue(3, ch3);
    }
}

void camLightDlg::slotSetAllLightValue(int lWhite, int lRed, int lGreen, int lBlue, int count)
{
    qDebug() << "recv light value" << lWhite << lRed << lGreen << lBlue << "count =" << count;
    //    qDebug() << "start setAllLightValue";
    ui->spinBox_LightValue->setValue(lWhite);
    ui->horizontalSlider_LightValue->setValue(lWhite);
    //    qDebug() << "22";
    setLightValue(0, lWhite);
    qDebug() << "setLightValue lWhite";
    m_lightInfo.white = lWhite;
    m_settings->setValue("/ConfigLight/LightValue", lWhite);

    ui->spinBox_LightValue1->setValue(lRed);
    ui->horizontalSlider_LightValue1->setValue(lRed);
    setLightValue(1, lRed);
    qDebug() << "setLightValue lRed";
    m_lightInfo.red = lRed;
    m_settings->setValue("/ConfigLight/Channel1", lRed);

    ui->spinBox_LightValue2->setValue(lGreen);
    ui->horizontalSlider_LightValue2->setValue(lGreen);
    setLightValue(2, lGreen);
    qDebug() << "setLightValue lGreen";
    m_lightInfo.green = lGreen;
    m_settings->setValue("/ConfigLight/Channel2", lGreen);

    //   qDebug() << "33";
    ui->spinBox_LightValue3->setValue(lBlue);
    ui->horizontalSlider_LightValue3->setValue(lBlue);
    setLightValue(3, lBlue);
    qDebug() << "setLightValue lBlue";
    m_lightInfo.blue = lBlue;
    m_settings->setValue("/ConfigLight/Channel3", lBlue);
    //    qDebug() << "44";
}

void camLightDlg::getLightValue(int &lWhite, int &lRed, int &lGreen, int &lBlue)
{
    lWhite = m_settings->value("/ConfigLight/LightValue").toInt();
    lRed   = m_settings->value("/ConfigLight/Channel1").toInt();
    lGreen = m_settings->value("/ConfigLight/Channel2").toInt();
    lBlue  = m_settings->value("/ConfigLight/Channel3").toInt();
    //    qDebug() << "camlight value" << lWhite << lRed << lGreen << lBlue;

    // connect(m_light->serial, &QSerialPort::channelReadyRead, this, [=]() {});
    //    QByteArray allData = m_light->serial->readAll();
    //    qDebug() << "allData.size()" << allData.size();
    //    for (int i = 0; i < allData.size(); i++) { qDebug() << "allData i =" << i << allData; }
}

void camLightDlg::resetLight()
{
    m_light->closeSerialPort(); //关闭串口
    openLight();                //重新打开光源
}

void camLightDlg::closeLight()
{
    if (!m_light) return;
    setLightValue(0, 0);
    setLightValue(1, 0);
    setLightValue(2, 0);
    setLightValue(3, 0);
    qDebug() << "close light.";
}

void camLightDlg::initInfo()
{
    if (!m_settings) return;
    //初始化UI界面
    initForm();
    m_light = RobotFactory().createConnect(ZWX_Robot);

    //打开光源
    openLight();
    //在程序exe目录下的"cameraLib"文件夹内，查找指定类型的相机dll文件[Dahua,Basler,HikVision]或者其他相机品牌
    QString exePath = QApplication::applicationDirPath();
    //相机类型 如HikVision
    QString cameraBrand = m_settings->value("/ConfigCamera/CamType").toString();
    QString pluginPath  = QString("%1/%2").arg(exePath).arg("cameraLib");
    qDebug() << "pluginPath:" << pluginPath;
    cameraSerial = m_settings->value("/ConfigCamera/CamSerial").toString();
    cameraModel  = m_settings->value("/ConfigCamera/CamModel").toString();
    qDebug() << "cameraSerial:" << cameraSerial << "cameraModel:" << cameraModel;
    //搜索已连接的相机，是否和配置文件设备名称和序列号一致的相机，赋给pCamera
    initCameraPlugin(pluginPath, cameraBrand);
    if (!pCamera) return;
    //返回这个dlg的父窗口给MainWindow
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());
    //如果camera有采集图像的信号，更新UI
    connect(pCamera, SIGNAL(grabImage(QImage)), mw, SLOT(grabImageUI(QImage)));
    //打开相机
    if (!pCamera->openCamera())
    {
        QMessageBox::information(this, "温馨提示", "相机打开失败");
        return;
    }
    //曝光
    double exposure = m_settings->value("/ConfigCamera/CamExposure").toDouble();
    //增益
    double gain = m_settings->value("/ConfigCamera/CamGain").toDouble();
    pCamera->setExposureValue(exposure);
    pCamera->setGainValue(gain);
    pCamera->startGrabImage(); //开始采集图像
    //获取相机的参数设置到UI上
    getCameraInfoValue();
}

void camLightDlg::initCameraPlugin(const QString &filePath, const QString &fileName)
{
    QDir dir;
    dir.setPath(filePath); //设置相机库的路径
    if (!dir.exists())
    {
        qDebug() << "cannot find camera path:" << dir.path();
        QMessageBox::information(nullptr, "温馨提示", "相机库路径不存在");
        return;
    }
    // entryList 返回目录中所有文件和目录的名称列表,依次传给filename
    foreach (QString filename, dir.entryList(QDir::Files))
    {
        if (filename.contains(filename)) //如果文件名称中有包含参数fileName的内容
        {
            // absoluteFilePath返回filename的绝对路径
            QPluginLoader *loader = new QPluginLoader(dir.absoluteFilePath(filename));
            //搜索相机dll
            if (!loader->load())
            {
                qDebug() << "cannot find camera dll:" << fileName;
                continue;
            }
            QObject *obj                 = loader->instance();                  //搜索实例
            esCamPlugin                  = qobject_cast<esCameraManage *>(obj); //动态转换QObject类的类型
            QVector<esCamera *> esCamera = esCamPlugin->updateCameraList();     //更新连接的相机列表
            if (esCamera.size() > 0)
            {
                for (int i = 0; i < esCamera.size(); ++i)
                {
                    if (esCamera[i])
                    {
                        //如果搜索到对应配置文件里相机设备的序列号和型号
                        if ((cameraSerial.compare(esCamera[i]->CameraSerialNumber().c_str()) == 0) && (cameraModel.compare(esCamera[i]->CameraModelName().c_str())) == 0)
                        {
                            qDebug() << "camera index:" << i;
                            pCamera = esCamera[i];
                            break;
                        }
                    }
                }
            }
            else
            {
                pCamera = nullptr;
                QMessageBox::information(nullptr, "温馨提示", "没有找到可用的相机");
                qDebug() << " camera size is zero!";
                return;
            }
            break;
        }
    }
}
void camLightDlg::setLightPort(const QString &arg1)
{
    if (!m_light) return;
    //设置界面上选择的值到配置文件
    m_settings->setValue("/ConfigLight/LightPort", arg1);
    m_light->closeSerialPort(); //关闭串口
    openLight();                //重新打开光源
}

void camLightDlg::setLightValue(int port, int value)
{
    if (!m_light) return;
    if (port < 0 || port > 6) return; // 0-light 1-ch1 2-ch2 3-ch3 最多有7个通道
    if (value < 0 || value > 255) return;

    static char lightValue[FrameLength] = {char(0XAA), char(0XFF), char(0X4C), 0, 0, 0, 0, 0, 0, 0};

    //将下标3，4，5，6赋值分别代表不同的通道
    lightValue[3 + port] = char(value); // 255
    QByteArray data(lightValue, 11);    //{170,255,76,255,0,0,0,0,0,0,0}一共11个
    //校验位
    auto crc       = m_light->crc16ForModbus(data); // crc = 2619
    lightValue[11] = char(crc >> 8);                // 10
    lightValue[12] = char(crc);                     // 59
    m_light->serial->write(lightValue, FrameLength);
}

// whitelight变化槽函数
void camLightDlg::on_horizontalSlider_LightValue_valueChanged(int value)
{
    ui->spinBox_LightValue->setValue(value);
    setLightValue(0, value);
    m_lightInfo.white = value;
    m_settings->setValue("/ConfigLight/LightValue", value);
}

void camLightDlg::on_spinBox_LightValue_valueChanged(int arg1)
{
    ui->horizontalSlider_LightValue->setValue(arg1);
    setLightValue(0, arg1);
    m_lightInfo.white = arg1;
    m_settings->setValue("/ConfigLight/LightValue", arg1);
}
// ch1变化槽函数
void camLightDlg::on_horizontalSlider_LightValue1_valueChanged(int value)
{
    ui->spinBox_LightValue1->setValue(value);
    setLightValue(1, value);
    m_lightInfo.white = value;
    m_settings->setValue("/ConfigLight/Channel1", value);
}

void camLightDlg::on_spinBox_LightValue1_valueChanged(int arg1)
{
    ui->horizontalSlider_LightValue1->setValue(arg1);
    setLightValue(1, arg1);
    m_lightInfo.red = arg1;
    m_settings->setValue("/ConfigLight/Channel1", arg1);
}
// ch2变化槽函数
void camLightDlg::on_horizontalSlider_LightValue2_valueChanged(int value)
{
    ui->spinBox_LightValue2->setValue(value);
    setLightValue(2, value);
    m_lightInfo.green = value;
    m_settings->setValue("/ConfigLight/Channel2", value);
}

void camLightDlg::on_spinBox_LightValue2_valueChanged(int arg1)
{
    ui->horizontalSlider_LightValue2->setValue(arg1);
    setLightValue(2, arg1);
    m_lightInfo.green = arg1;
    m_settings->setValue("/ConfigLight/Channel2", arg1);
}
// ch3变化槽函数
void camLightDlg::on_horizontalSlider_LightValue3_valueChanged(int value)
{
    ui->spinBox_LightValue3->setValue(value);
    setLightValue(3, value);
    m_lightInfo.blue = value;
    m_settings->setValue("/ConfigLight/Channel3", value);
}

void camLightDlg::on_spinBox_LightValue3_valueChanged(int arg1)
{
    ui->horizontalSlider_LightValue3->setValue(arg1);
    setLightValue(3, arg1);
    m_lightInfo.blue = arg1;
    m_settings->setValue("/ConfigLight/Channel3", arg1);
}

void camLightDlg::getCameraInfoValue()
{
    if (!pCamera) return;
    //设置厂商，型号，序列号
    ui->cameravendor->setText(pCamera->CameraVendorName().c_str());
    ui->cameramodel->setText(pCamera->CameraModelName().c_str());
    ui->cameraserial->setText(pCamera->CameraSerialNumber().c_str());
    double expouse = 0.0, gain = 0.0;
    //读取曝光，增益
    pCamera->exposureValue(expouse);
    pCamera->gainValue(gain);
    //设置UI
    ui->expouseslider->setValue(int(expouse));
    ui->expousebox->setValue(expouse);
    ui->gainslider->setValue(int(gain));
    ui->gainbox->setValue(gain);
    int type = -1;
    pCamera->triggerType(type);
    if (type == 0)
    {
        ui->singlemodel->setChecked(true);
        isContinus = false;
    }
    else if (type == 1)
    {
        ui->continuousmodel->setChecked(true);
        isContinus = true;
    }
    else if (type == 2)
    {
        ui->externalmodel->setChecked(true);
        isContinus = false;
    }
}

//单帧触发
void camLightDlg::on_singlemodel_clicked()
{
    if (!pCamera) return;
    int type   = -1;
    isContinus = false;
    pCamera->triggerType(type);
    if (type != 0) { pCamera->setTriggerType(esCamera::SingleFrame); }
    else
    {
        qDebug() << "trigger type:" << type;
    }
}

void camLightDlg::on_continuousmodel_clicked()
{
    qDebug() << "on_continuousmodel_clicked";
    if (!pCamera) { return; }
    isContinus = true;
    int type   = -1;
    pCamera->triggerType(type);
    if (type != 1)
    {
        pCamera->setTriggerType(esCamera::Continuous);
        qDebug() << "!=1 trigger type:" << type;
    }
    else
    {
        qDebug() << "trigger type:" << type;
    }
}

void camLightDlg::on_externalmodel_clicked()
{
    if (!pCamera) return;
    isContinus = false;
    int type   = -1;
    pCamera->triggerType(type);
    if (type != 2) { pCamera->setTriggerType(esCamera::External); }
    else
    {
        qDebug() << "trigger type:" << type;
    }
}

void camLightDlg::on_expouseslider_valueChanged(int value)
{
    if (!pCamera) return;
    ui->expousebox->setValue(static_cast<double>(value));
    if (!pCamera->setExposureValue(static_cast<double>(value)))
    {
        qDebug() << "on_expouseslider_valueChanged fail." << value;
        return;
    }
    m_settings->setValue("/ConfigCamera/CamExposure", value);
}

void camLightDlg::on_expousebox_editingFinished()
{
    if (!pCamera) return;
    double value = ui->expousebox->value();
    ui->expouseslider->setValue(static_cast<int>(value));
    if (!pCamera->setExposureValue(value))
    {
        qDebug() << "on_expousebox_editingFinished fail." << value;
        return;
    }
    m_settings->setValue("/ConfigCamera/CamExposure", ui->expousebox->value());
}

void camLightDlg::on_gainslider_valueChanged(int value)
{
    if (!pCamera) return;
    ui->gainbox->setValue(static_cast<double>(value));
    if (!pCamera->setGainValue(static_cast<double>(value)))
    {
        qDebug() << "on_gainslider_valueChanged fail." << value;
        return;
    }
    m_settings->setValue("/ConfigCamera/CamGain", value);
}

void camLightDlg::on_gainbox_editingFinished()
{
    if (!pCamera) return;
    double value = ui->gainbox->value();
    ui->gainslider->setValue(static_cast<int>(value));
    if (!pCamera->setGainValue(value))
    {
        qDebug() << "on_gainbox_editingFinished fail." << value;
        return;
    }
    m_settings->setValue("/ConfigCamera/CamGain", ui->gainbox->value());
}

void camLightDlg::closeDevice()
{
    if (!m_light)
    {
        qDebug() << "m_light is zero.";
        return;
    }
    m_light->writeSpeedValueToPort(0);
    if (!pCamera) return;
    pCamera->stopGrabImage();
    pCamera->closeCamera();
    qDebug() << "closeDevice finshed.";
}
