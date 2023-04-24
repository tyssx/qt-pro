#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <QPushButton>
#include <QCheckBox>
#include <QDesktopServices>

int MainWindow::num = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("轴棒检测工具");

    //设置背景色为白色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);

    g_handle = NULL;
    m_Acq = NULL;
    m_AcqDevice = NULL;
    m_Buffers = NULL;
    m_Xfer = NULL;
    m_View = NULL;
    mAcqConfig = new AcqConfig;

    //创建保存文件的路径
    exepath = QCoreApplication::applicationDirPath();
    qDebug() << "exepath" << exepath;
    srcpath = exepath + "/img_src";
    objpath = exepath + "/img_obj";
    QDir sdir(srcpath);
    if(!sdir.exists())
        sdir.mkpath(srcpath); //路径不存在，创建
    QDir odir(objpath);
    if(!odir.exists())
        odir.mkpath(objpath); //路径不存在，创建

    //设置控件隐藏不可见，实际还会占布局位置
    ui->label_6->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_8->setVisible(false);
    ui->label_9->setVisible(false);
    ui->lineEditXimode->setVisible(false);
    ui->lineEditXivalue->setVisible(false);
    ui->lineEditYivalue->setVisible(false);

    ui->comboBoxIP->addItem("192.168.0.11");
    ui->comboBoxIP->addItem("127.0.0.1");

    //设置按钮灰色不可用
    ui->actionactionSnap->setEnabled(false);
    ui->actionactionGrab->setEnabled(false);
    ui->actionactionFreeze->setEnabled(false);
    setButtonEnabled(false);

    //工具栏新建一个checkbox功能
    checkIsTest = new QCheckBox(this);
    checkIsTest->setText(QString("是否检测"));
    ui->mainToolBar->addWidget(checkIsTest);
    checkIsTest->setChecked(true); //默认选中

    //获取相机配置文件，连接采集及初始化采集目标
    connect(ui->actionactionOpenCCF,&QAction::triggered,this,&MainWindow::getCcfFile);
    //单张采集
    connect(ui->actionactionSnap,&QAction::triggered,this,&MainWindow::SlotSnap);
    //连续采集
    connect(ui->actionactionGrab,&QAction::triggered,this,&MainWindow::SlotGrab);
    //停止采集
    connect(ui->actionactionFreeze,&QAction::triggered,this,&MainWindow::SlotFreeZe);
    //判断线程是否存在，并且所有的照片已经处理完成，关闭线程，释放内存
    connect(ui->actionactionFreeze,&QAction::triggered,this,&MainWindow::closeImgDealThread);

    //运动控制连接
    connect(ui->pushBtnConnect,&QPushButton::clicked,this,&MainWindow::conController);
    //运动控制关闭
    connect(ui->pushBtnClose,&QPushButton::clicked,this,[=](){
        if(g_handle != NULL)
        {
            int ret = ZAux_Close(g_handle);
            g_handle = NULL;
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("连接已断开:")+QString::number(ret));

            ui->pushBtnConnect->setEnabled(true);
            //设置按钮灰色不可用
            setButtonEnabled(false);
        }
    });

    //水平运动，原点在右侧，这里的左右是以靠近原点为左，远离原点为右
    connect(ui->pushBtnLeft,&QPushButton::clicked,this,&MainWindow::setLeftShift);
    connect(ui->pushBtnRight,&QPushButton::clicked,this,&MainWindow::setRightShift);
    //按键长按，优先级pressed>released>clicked，如果加上clicked，点击一次按键，会造成槽函数执行两次，所以需要去掉clicked的连接
    QTimer *leftTimer = new QTimer(this);
    connect(leftTimer,&QTimer::timeout,this,&MainWindow::setLeftShift);
    connect(ui->pushBtnLeft,&QPushButton::pressed,this,[=](){
        leftTimer->start(550);
    });
    connect(ui->pushBtnLeft,&QPushButton::released,this,[=](){
        leftTimer->stop();
    });

    QTimer *rightTimer = new QTimer(this);
    connect(rightTimer,&QTimer::timeout,this,&MainWindow::setRightShift);
    connect(ui->pushBtnRight,&QPushButton::pressed,this,[=](){
        rightTimer->start(550);
    });
    connect(ui->pushBtnRight,&QPushButton::released,this,[=](){
        rightTimer->stop();
    });

    //水平方向返回原点
    connect(ui->pushBtnOrigin,&QPushButton::clicked,this,&MainWindow::setLeftRightOrigin);
    connect(ui->pushBtnXStop,&QPushButton::clicked,this,[=](){
        if (g_handle == NULL)
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString(tr("左右停止失败,请检查是否已连接!")));
        }
        ZAux_Direct_Singl_Cancel(g_handle, 0, 2);        //
    });

    connect(ui->pushBtnXZero,&QPushButton::clicked,this,[=](){
        if (g_handle == NULL)
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString(tr("左右置零失败,请检查是否已连接!")));
        }
        ZAux_Direct_SetDpos(g_handle, 0, 0);        //设置零点
    });

    //顺时针逆时针运动，顺时针方向数值增大，逆时针方向数值减小
    //connect(ui->pushBtnClockwise,&QPushButton::clicked,this,&MainWindow::setClockWise);
    //connect(ui->pushBtnAnticlockwise,&QPushButton::clicked,this,&MainWindow::setAntiClockWise);
    //按键长按，优先级pressed>released>clicked，如果加上clicked，点击一次按键，会造成槽函数执行两次，所以需要去掉clicked的连接
    QTimer *clockTimer = new QTimer(this);
    connect(clockTimer,&QTimer::timeout,this,&MainWindow::setClockWise);
    connect(ui->pushBtnClockwise,&QPushButton::pressed,this,[=](){
        clockTimer->start(250);
    });
    connect(ui->pushBtnClockwise,&QPushButton::released,this,[=](){
        clockTimer->stop();
    });

    QTimer *anticlockTimer = new QTimer(this);
    connect(anticlockTimer,&QTimer::timeout,this,&MainWindow::setAntiClockWise);
    connect(ui->pushBtnAnticlockwise,&QPushButton::pressed,this,[=](){
        anticlockTimer->start(250);
    });
    connect(ui->pushBtnAnticlockwise,&QPushButton::released,this,[=](){
        anticlockTimer->stop();
    });

    //顺逆时针位置置0
    connect(ui->pushBtnYZero,&QPushButton::clicked,this,[=](){
        if (g_handle == NULL)
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString(tr("顺逆置零失败,请检查是否已连接!")));
        }
        ZAux_Direct_SetDpos(g_handle, 1, 0);        //设置零点
    });

    //顺时针运动，停止
    connect(ui->pushBtnClockStart,&QPushButton::clicked,this,[=](){

        ui->pushBtnAnticlockStart->setEnabled(false);
        if(ui->pushBtnClockStart->text() == "顺时转动")
        {
            ZAux_Direct_Singl_Vmove(g_handle,1,1); //单轴连续运动
            ui->pushBtnClockStart->setText("停止转动");
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("开始顺时针转动;"));

        }
        else if(ui->pushBtnClockStart->text() == "停止转动")
        {
            ZAux_Direct_Singl_Cancel(g_handle, 1, 2); //单轴停止运动
            ui->pushBtnClockStart->setText("顺时转动");
            ui->pushBtnAnticlockStart->setEnabled(true);
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("停止运动"));
        }

    });

    //逆时针运动，停止
    connect(ui->pushBtnAnticlockStart,&QPushButton::clicked,this,[=](){

        ui->pushBtnClockStart->setEnabled(false);
        if(ui->pushBtnAnticlockStart->text() == "逆时转动")
        {
            ZAux_Direct_Singl_Vmove(g_handle,1,-1); //单轴连续运动
            ui->pushBtnAnticlockStart->setText("停止转动");
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("开始逆时针转动;"));
        }
        else if(ui->pushBtnAnticlockStart->text() == "停止转动")
        {
            ZAux_Direct_Singl_Cancel(g_handle, 1, 2); //单轴停止运动
            ui->pushBtnAnticlockStart->setText("逆时转动");
            ui->pushBtnClockStart->setEnabled(true);
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("停止运动"));
        }
    });

    //连接后，启动定时器，获取实时位置信息
    connect(ui->pushBtnConnect,&QPushButton::clicked,this,[=](){
        //定时器，当500ms到了之后发出一个timeout信号
        QTimer *timer = new QTimer(this);
        if(g_handle != NULL)
        {
            timer->start(100);
            connect(timer,&QTimer::timeout,this,[=](){
                //获取当前的实时位置
                float xpos = 0.00;
                ZAux_Direct_GetDpos(g_handle,0,&xpos);
                //将float转换为QString
                ui->lineEditXPos->setText(QString("%1").arg(xpos));
            });
        }

        //定时器，当500ms到了之后发出一个timeout信号
        QTimer *timer2 = new QTimer(this);
        if(g_handle != NULL)
        {
            timer2->start(100);
            connect(timer2,&QTimer::timeout,this,[=](){
                //获取当前的实时位置
                float ypos = 0.00;
                ZAux_Direct_GetDpos(g_handle,1,&ypos);
                //将float转换为QString
                ui->lineEditYPos->setText(QString("%1").arg(ypos));
            });
        }
    });

    //自动检测运动
    connect(ui->pushBtnAutoSport,&QPushButton::clicked,this,[=](){

        emit autoSportSignal(g_handle);
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("开始自动检测!"));

    });

    //停止运动，需要一次一次结束
    connect(ui->pushBtnStopAuto,&QPushButton::clicked,this,[=](){

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("正在停止当前运动..."));


        QTimer *timer = new QTimer(this);
        timer->start(200);
        int n = 0; //lambda表达式中，捕获的变量n默认是不可变的,需要加上mutable。
        connect(timer,&QTimer::timeout,this,[=]()mutable{
            int run_state = 1; //轴运动状态
//            qDebug() << "n = " << n;
            ZAux_Direct_GetIfIdle(g_handle,0,&run_state);
            if(run_state == 0)
            {
                ZAux_Direct_Singl_Cancel(g_handle, 0, 2);
//                qDebug() << "0 cancel";
            }
            ZAux_Direct_GetIfIdle(g_handle,1,&run_state);
            if(run_state == 0)
            {
                ZAux_Direct_Singl_Cancel(g_handle, 1, 2);
//                qDebug() << "1 cancel";
            }
            if(n++ == 16)
            {
                timer->stop();
                qDebug() << " timer->stop";
                QDateTime current_date_time =QDateTime::currentDateTime();
                QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
                ui->textEditLog->append(current_date + QString("已停止当前运动!"));
//                emit leftRightOriginSignal(g_handle,4);
            }
        });
    });

    //打开所在路径的资源管理器
    connect(ui->pushBtnSrcDir,&QPushButton::clicked,this,[=](){
        QDesktopServices::openUrl(QUrl(srcpath, QUrl::TolerantMode));
    });
    connect(ui->pushBtnObjDir,&QPushButton::clicked,this,[=](){
        QDesktopServices::openUrl(QUrl(objpath, QUrl::TolerantMode));
    });

}

MainWindow::~MainWindow()
{
    delete ui;

    ZAux_Close(g_handle);
    g_handle = NULL;

    DestroyObjects();

    // Delete all objects
    if (m_Xfer)			delete m_Xfer;
    if (m_View)			delete m_View;
    if (m_Buffers)		delete m_Buffers;
    if (m_Acq)			delete m_Acq;

    if(m_ImgDealThread)
    {
        delete  m_ImgDealThread;
        m_ImgDealThread = NULL;
    }
    if(m_ImgDeal)
    {
        delete  m_ImgDeal;
        m_ImgDeal = NULL;
    }

    if(m_sptContThread)
    {
        delete  m_sptContThread;
        m_sptContThread = NULL;
    }
    if(m_SptControl)
    {
        delete  m_SptControl;
        m_SptControl = NULL;
    }
}

void MainWindow::setButtonEnabled(bool flag)
{
    ui->pushBtnClose->setEnabled(flag);
    ui->pushBtnLeft->setEnabled(flag);
    ui->pushBtnRight->setEnabled(flag);
    ui->pushBtnOrigin->setEnabled(flag);
    ui->pushBtnClockwise->setEnabled(flag);
    ui->pushBtnAnticlockwise->setEnabled(flag);
    ui->pushBtnClockStart->setEnabled(flag);
    ui->pushBtnAnticlockStart->setEnabled(flag);
    ui->pushBtnYZero->setEnabled(flag);
    ui->pushBtnXStop->setEnabled(flag);
    ui->pushBtnXZero->setEnabled(flag);
    ui->pushBtnAutoSport->setEnabled(flag);
    ui->pushBtnStopAuto->setEnabled(flag);
}
void MainWindow::getCcfFile()
{
    //获取相机配置文件
    ccfpath = QFileDialog::getOpenFileName(this,"打开CCF文件",QCoreApplication::applicationDirPath(),"CCF文件(*.ccf;*.CCF)");
    if(ccfpath.isEmpty())
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("你未选择相机配置文件，请选择!"));
        return ;
    }
    qDebug() << "ccfpath " << ccfpath;
    //获取已连接的采集服务器（Linea_M4096-7um_1），采集设备（12205585）
    if(!this->LoadGigeCamera(ccfpath.toUtf8().data()))
    {
        qDebug() << "MainWindow:camera connect failed";
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("相机连接失败，请重试!"));
    }
    else
    {
        //初始化采集目标
        if(this->InitObjects())
        {
            ui->actionactionSnap->setEnabled(true);
            ui->actionactionGrab->setEnabled(true);
            ui->actionactionFreeze->setEnabled(true);
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("相机连接成功，请开始采集!"));
        }
        else
        {
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
            ui->textEditLog->append(current_date + QString("相机初始化失败，请重试!"));
        }
    }
}

void MainWindow::SlotSnap()
{
    if(m_Xfer->Snap())
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("单张采集!"));
    }
    if(checkIsTest->isChecked())
    {
        num = 0;
        m_ImgDeal = new ImageDeal();
        m_ImgDealThread = new QThread();
        m_ImgDeal->moveToThread(m_ImgDealThread);
        qDebug()<< "main thread start,thread id = " << QThread::currentThreadId();
        m_ImgDealThread->start();
        qDebug() << "start m_ImgDealThread";
        //结构体必须要注册
        qRegisterMetaType<ImageDeal::Parameter>("ImageDeal::Parameter");
        connect(this,&MainWindow::haveCameraImage,m_ImgDeal,&ImageDeal::imageDealSlot);
        connect(m_ImgDeal,&ImageDeal::singleFinishSingnal,this,&MainWindow::showResultImage);
        //单张采集添加一个释放
        connect(m_ImgDeal,&ImageDeal::singleFinishSingnal,this,[=](){

            qDebug() << "m_ImgDealThread quit";
            m_ImgDealThread->quit(); //停止线程
            m_ImgDealThread->wait(); //等待线程处理完手头工作
            if(m_ImgDealThread)
            {
                delete  m_ImgDealThread;
                m_ImgDealThread = NULL;
            }
            if(m_ImgDeal)
            {
                delete  m_ImgDeal;
                m_ImgDeal = NULL;
            }
        });
    }

}

void MainWindow::SlotGrab()
{
    ui->actionactionOpenCCF->setEnabled(false);
    ui->actionactionSnap->setEnabled(false);
    ui->actionactionGrab->setEnabled(false);
    //ui->actionactionFreeze->setEnabled(true);
    if(m_Xfer->Grab())
    {
        qDebug() << "start Grab";
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("连续采集!"));
    }

    num = 0;
    if(checkIsTest->isChecked())
    {
        m_ImgDeal = new ImageDeal();
        m_ImgDealThread = new QThread();
        m_ImgDeal->moveToThread(m_ImgDealThread);
        qDebug()<< "main thread start,thread id = " << QThread::currentThreadId();
//        connect(this,&MainWindow::startGrabSignal,m_ImgDeal,&ImageDeal::grabStartSlot);
        m_ImgDealThread->start();
//        emit startGrabSignal();
        qDebug() << "start m_ImgDealThread";
        //结构体必须要注册
        qRegisterMetaType<ImageDeal::Parameter>("ImageDeal::Parameter");
        connect(this,&MainWindow::haveCameraImage,m_ImgDeal,&ImageDeal::imageDealSlot);
        connect(m_ImgDeal,&ImageDeal::singleFinishSingnal,this,&MainWindow::showResultImage);
    }

}

void MainWindow::showResultImage(long time,QImage imgResult,int count)
{
    qDebug() << "++++++Recv SingleFinishSingnal++++++";
    qDebug() << "第 " << count << " 张的检测时间为 " << time << " ms";
    ui->lineEditTime->setText(QString::number(time)+"ms");
    ui->labelObj->setPixmap(QPixmap::fromImage(imgResult).scaled(ui->labelObj->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    ui->labelDealNum->setText("第 " + QString::number(count+1) + " 张");
    update();
    d_Num = count;
    imgResult.save(objpath + "\\" +QString::number(count)+".jpg");
}

void MainWindow::SlotFreeZe()
{
    // Stop grab
    ui->actionactionOpenCCF->setEnabled(true);
    ui->actionactionSnap->setEnabled(true);
    ui->actionactionGrab->setEnabled(true);
    if(m_Xfer->Freeze())
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("已停止采集!"));
    }
    if (!m_Xfer->Wait(5000))
        printf("Grab could not stop properly.\n");
    qDebug() << "FreeZe end";
}

void MainWindow::closeImgDealThread()
{
    if(m_ImgDealThread != NULL || m_ImgDeal != NULL)
    {
        QTimer *timer = new QTimer(this);
        static int n = 0;
        timer->start(500);
        connect(timer,&QTimer::timeout,this,[=](){
            qDebug() << "actionactionFreeze d_Num " << d_Num << "num-1" << num-1 ;
            //判断从线程获取到的已处理完成的图像序号d_Num，和采集到的图像序号num是否一致，
            //如果一致证明所有图像处理完成。因为num完成后会加1，所以需要减去1。
            if(d_Num == num-1)
            {
                qDebug() << "m_ImgDealThread quit";
                m_ImgDealThread->quit(); //停止线程
                m_ImgDealThread->wait(); //等待线程处理完手头工作

                qDebug() << "delete m_ImgDealThread m_ImgDeal";
                delete  m_ImgDealThread;
                delete  m_ImgDeal;
                m_ImgDealThread = NULL;
                m_ImgDeal = NULL;

                timer->stop();
            }
            n++;
            if(n == 20)
            {
                qDebug() << "timer->stop";
                qDebug() << "m_ImgDealThread quit";
                m_ImgDealThread->quit(); //停止线程
                m_ImgDealThread->wait(); //等待线程处理完手头工作

                qDebug() << "delete m_ImgDealThread m_ImgDeal";
                delete  m_ImgDealThread;
                delete  m_ImgDeal;
                m_ImgDealThread = NULL;
                m_ImgDeal = NULL;

                timer->stop();
            }
        });
    }
}

bool MainWindow::LoadGigeCamera(const char *ccfPath)
{
    acqDeviceNumber = 0;
    memset(acqServerName,0,sizeof (acqServerName));
    memset(acqDeviceName,0,sizeof (acqDeviceName));
    memset(configFilename,0,sizeof (configFilename));
    memcpy(configFilename,ccfPath,strlen(ccfPath)+1);
    if(!mAcqConfig->GetCorAcqDeviceOptionsFromQuestions(acqServerName, acqDeviceName, &acqDeviceNumber))
    {
        qDebug() << "相机连接失败";
        return false;
    }
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("采集服务名称：") + QString(acqServerName) + "    " +
                                           QString("采集设备名称：") + QString(acqDeviceName));
    return true;
}


bool MainWindow::InitObjects()
{
    SapLocation sLoc(acqServerName,acqDeviceNumber);

    if (SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcq) > 0)
    {
        qDebug() << "--ResourceAcq--";
        m_Acq		= new SapAcquisition(sLoc, configFilename);  //采集者
        m_Buffers	= new SapBuffer(1, m_Acq);                   //采集的buffer
        m_View		= new SapView(m_Buffers, SapHwndAutomatic);  //采集显示，例子中的，不用
        m_Xfer		= new SapAcqToBuf(m_Acq, m_Buffers, AcqCallback, m_View);

        // Create acquisition object
        if (m_Acq && !*m_Acq && !m_Acq->Create())
        {
            DestroyObjects();
            return false;
        }
    }
    else if (SapManager::GetResourceCount(acqServerName, SapManager::ResourceAcqDevice) > 0)
    {
        qDebug() << "--ResourceAcqDevice--"; //执行这里
        if (strcmp(configFilename, "NoFile") == 0)
        {
            qDebug() << "NoFile";
            m_AcqDevice = new SapAcqDevice(sLoc, FALSE);
        }
        else
        {
            qDebug() << "Have File";
            m_AcqDevice = new SapAcqDevice(sLoc, configFilename);  //采集设备
        }
        m_Buffers = new SapBufferWithTrash(2, m_AcqDevice);        //采集到的数据
        //        m_View    = new SapView(m_Buffers, SapHwndAutomatic);
        //        m_Xfer    = new SapAcqDeviceToBuf(m_AcqDevice, m_Buffers, XferCallback, m_View);
        //在SapAcqDeviceToBuf这个注册回调函数里调用回调函数不停的获取buffer，外面自己用回调函数处理收到buffer的过程，并且传递一个参数this传进去
        m_Xfer    = new SapAcqDeviceToBuf(m_AcqDevice, m_Buffers, XferCallback, this);  //不停的采集，注册回调函数

        // Create acquisition object
        if (m_AcqDevice && !*m_AcqDevice && !m_AcqDevice->Create())
        {
            DestroyObjects();
            return false;
        }
    }

    qDebug() << "11";
    // Create buffer object
    if (m_Buffers && !*m_Buffers)
    {
        if(!m_Buffers->Create())
        {
            qDebug() << "111";
            DestroyObjects();
            return false;
        }
        // Clear all buffers
        qDebug() << "112";
        m_Buffers->Clear();
    }

    qDebug() << "12";
    // Create view object
    if (m_View && !*m_View && !m_View->Create())
    {
        qDebug() << "121";
        DestroyObjects();
        return false;
    }

    qDebug() << "13";
   // Set next empty with trash cycle mode for transfer
    if (m_Xfer && m_Xfer->GetPair(0))
    {
        if (!m_Xfer->GetPair(0)->SetCycleMode(SapXferPair::CycleNextWithTrash))
        {
            qDebug() << "131";
            DestroyObjects();
            return false;
        }
    }

    qDebug() << "14";
    // Create transfer object
    if (m_Xfer && !*m_Xfer && !m_Xfer->Create())
    {
        qDebug() << "141";
        DestroyObjects();
        return false;
    }
    return true;
}


void MainWindow::AcqCallback(SapXferCallbackInfo *pInfo)
{
    SapView *pView= (SapView *) pInfo->GetContext();
    pView->Show();
}

//在回调函数的
void MainWindow::XferCallback(SapXferCallbackInfo *pInfo)
{
    MainWindow *camera = (MainWindow *)pInfo->GetContext();
    if(!pInfo->IsTrash())
    {
        void *pDataAddr = nullptr;
        int width = camera->m_Buffers->GetWidth();
        int height = camera->m_Buffers->GetHeight();
        //int depth = camera->m_Buffers->GetPixelDepth();
        //qDebug() << "width " << width << " height " << height  << " depth " << depth;

        SapFormat format = -1;
        SapBuffer::Type type = -1;
        int count = 0;
        camera->m_Buffers->GetParameters(&count, &width, &height, &format, &type);
        qDebug() << "count " << count << " width " << width << " height " << height  << " format " << format << " type " << type;

        if(!camera->m_Buffers->GetAddress(&pDataAddr))
        {
            qDebug() << "Unable to get image!";
        }
        else
        {
            qDebug() << "--------->get image!<--------";
            QImage image = QImage((uchar *)pDataAddr,width,height,QImage::Format_Grayscale8); //8bit灰度图 Format_Indexed8
            camera->par.e1 = camera->ui->spinBlackTolerance->value();
            camera->par.e3 = camera->ui->spinWhiteTolerance->value();
            bool bFlag = camera->ui->checkBoxBlack->isChecked();
            if(bFlag)
            {
                camera->par.blackff = 1;
            }
            else {
                camera->par.blackff = 0;
            }
            bool wFlag = camera->ui->checkBoxWhite->isChecked();
            if(wFlag)
            {
                camera->par.whiteff = 1;
            }
            else {
                camera->par.whiteff = 0;
            }
            camera->par.vArea = camera->ui->spinAreaThreshold->value();
            emit camera->haveCameraImage(image,camera->par,num);

            QString str = camera->srcpath + "\\" + QString::number(camera->num++) + ".jpg";
            image.save(str);
            camera->ui->labelGetNum->setText("第 " + QString::number(num) + " 张");
            //按照比例缩放在label上
            camera->ui->labelSrc->setPixmap(QPixmap::fromImage(image).scaled(camera->ui->labelSrc->size(),
                                                                          Qt::KeepAspectRatio,
                                                                          Qt::SmoothTransformation));
        }
    }
}

void MainWindow::DestroyObjects()
{
    // Destroy transfer object
    if (m_Xfer && *m_Xfer) m_Xfer->Destroy();

    // Destroy view object
    if (m_View && *m_View) m_View->Destroy();

    // Destroy buffer object
    if (m_Buffers && *m_Buffers) m_Buffers->Destroy();

    // Destroy acquisition object
    if (m_AcqDevice && *m_AcqDevice) m_AcqDevice->Destroy();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("采集失败，请重试!"));
}


//通过IP连接到控制器
void MainWindow::conController()
{
    qDebug() << "conController";
    //char ipAdd[] = "127.0.0.1";

    if(g_handle != NULL)
    {
        qDebug() << "g_handle != NULL";
        ZAux_Close(g_handle);

        g_handle = NULL;
    }
    QString qsIP = ui->comboBoxIP->currentText();

    int ret = ZAux_OpenEth(qsIP.toUtf8().data(),&g_handle);

    qDebug() << ret;

    if(ret == 0)
    {
        qDebug() << "33";
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString("连接成功,ZAux_OpenEth: ")+QString::number(ret));
        //设置按钮可用
        setButtonEnabled(true);
        ui->pushBtnConnect->setEnabled(false);

        //初始化轴参数
        for(int i=0 ;i<2;i++)
        {
            //设定轴类型 1-脉冲轴类型
            ZAux_Direct_SetAtype(g_handle, i, 1);

            //设定脉冲模式及逻辑方向（脉冲+方向）
//            ZAux_Direct_SetInvertStep(g_handle, i, 0);
            if(i==0)
            {
                QString sxv = ui->lineEditXivalue->text();
                int xv = sxv.toInt();
                qDebug() << "SetInvertStep xv " << xv;
                ZAux_Direct_SetInvertStep(g_handle, i, xv);
            }
            if(i==1)
            {
                QString syv = ui->lineEditYivalue->text();
                int yv = syv.toInt();
                qDebug() << "SetInvertStep yv " << yv;
                ZAux_Direct_SetInvertStep(g_handle, i, yv);
            }

            //设置脉冲当量	1表示以一个脉冲为单位 ，设置为1MM的脉冲个数，这度量单位为MM
            ZAux_Direct_SetUnits(g_handle, i, 100); //设置轴 i 轴参数以 100 个脉冲为单位

            //设定速度，加减速
            ZAux_Direct_SetSpeed(g_handle, i, 300);  //设置轴 i 速度为 200units/s

            if(i == 1)
            {
                ZAux_Direct_SetSpeed(g_handle, i, 50);  //设置轴 1 速度为 50units/s
            }

            ZAux_Direct_SetAccel(g_handle, i, 2000); //设置轴 i 加速度为 2000units/s/s
            ZAux_Direct_SetDecel(g_handle, i, 2000); //设置轴 i 减速度为 2000units/s/s

            //设定S曲线时间 设置为0表示梯形加减速
            ZAux_Direct_SetSramp(g_handle, i, 0);
        }
        //开启运动线程
        m_SptControl = new SportControl();
        m_sptContThread = new QThread();
        m_SptControl->moveToThread(m_sptContThread);
        m_sptContThread->start();

        connect(this,&MainWindow::singleSportSignal,m_SptControl,&SportControl::singleAxisSportSlot);
        connect(this,&MainWindow::leftRightOriginSignal,m_SptControl,&SportControl::leftRightOriginSlot);
        connect(this,&MainWindow::autoSportSignal,m_SptControl,&SportControl::autoSportSlot);
//        connect(this,&MainWindow::stopAutoSportSignal,m_SptControl,&SportControl::stopAutoSportSlot);
    }
    else {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
        ui->textEditLog->append(current_date + QString(tr("连接失败,请检查!"))+QString::number(ret));
    }
}

//将X轴往减小的方向移动
void MainWindow::setLeftShift()
{
    qDebug() << "leftShift";
    emit singleSportSignal(g_handle,0,-100);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("左移100"));
}

//将X轴往增大的方向移动
void MainWindow::setRightShift()
{
    qDebug() << "rightShift";
    emit singleSportSignal(g_handle,0,100);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("右移100"));
}

//将X轴回到原点位置
void MainWindow::setLeftRightOrigin()
{
    QString sxm = ui->lineEditXimode->text();
    int xm = sxm.toInt();
    qDebug() << "Singl_Datum xm " << xm;
    emit leftRightOriginSignal(g_handle,xm);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("正在回零!"));
}

//顺时针方向移动
void MainWindow::setClockWise()
{
    qDebug() << "clockWise";

    emit singleSportSignal(g_handle,1,10);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("顺时针移10"));
}

//逆时针方向移动
void MainWindow::setAntiClockWise()
{
    qDebug() << "antiClock";

    emit singleSportSignal(g_handle,1,-10);
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss: ");
    ui->textEditLog->append(current_date + QString("逆时针移10"));
}

