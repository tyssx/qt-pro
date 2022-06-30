#include "mainwindow.h"

#include "measure/MeasureAssist.h" //评价函数
#include "optimaization.h"         //优化函数
#include "ui_mainwindow.h"
#include "waitdialog.h"

#include <QDockWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>

int MainWindow::num = 0;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //    this->resize(800, 620);
    this->resize(1500, 620);

    // exe所在的路径
    QString exePath = QApplication::applicationDirPath();
    //配置文件所在路径
    QString filePath = QString("%1/%2/%3").arg(exePath).arg("config").arg("defaultConfig.ini");
    m_settings       = new QSettings(filePath, QSettings::IniFormat, this); // QSetting用来处理ini文件

    QPushButton *btnAuto = new QPushButton("AL", ui->mainToolBar);
    btnAuto->setVisible(false);

    QLabel *m_Label = new QLabel(ui->mainToolBar);
    m_Label->setText("查找次数:");

    m_ComboBox = new QComboBox(ui->mainToolBar);
    m_ComboBox->addItem("100");
    m_ComboBox->addItem("200");
    m_ComboBox->addItem("300");
    m_ComboBox->addItem("400");

    //存储接口
    m_store       = new CStorageInterface();
    m_camLightDlg = new camLightDlg(this);
    m_camLightDlg->setStoreCtrl(m_store);
    m_camLightDlg->setQSettingsCtrl(m_settings);
    m_camLightDlg->initInfo();

    QAction *action_saveImage = new QAction;
    action_saveImage->setText("图像保存");

    //功能菜单添加
    QMenu *menu = new QMenu(this);
    menu->addAction(ui->action_set);
    menu->addAction(action_saveImage);

    // 在工具栏添加一个button
    QPushButton *menuButton = new QPushButton("功能", ui->mainToolBar);
    menuButton->setMenu(menu); //将功能添加到菜单Button上面

    //将AL，menu,label,combobox添加到水平布局
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0); //设置边界距离
    layout->addWidget(btnAuto);
    layout->addWidget(menuButton);
    layout->addWidget(m_Label);
    layout->addWidget(m_ComboBox);

    QWidget *w = new QWidget();
    w->setLayout(layout);
    ui->mainToolBar->addWidget(w);
    ui->mainToolBar->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->mainToolBar->setMovable(false);

    //新添加部分
    m_ImageForm = new ImageForm(this);
    setCentralWidget(m_ImageForm);

    //停靠窗口
    dockWidget = new QDockWidget("评价折线图", this);
    dockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable); //设置停靠窗口特性，可浮动，可关闭
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
    dockWidget->setMinimumSize(700, 580);
    // dockWidget->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea); //设置可停靠区域为主窗口上边和右边
    //    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

    //创建图表
    createChart();

    maxlabel = new QLabel("最优光源值:             评价值:             ", this);
    ui->statusBar->addWidget(maxlabel);
    //    lightlabel = new QLabel("实时光源值:", this);
    //    ui->statusBar->addWidget(lightlabel);
    curlabel = new QLabel("当前评价值:", this);
    ui->statusBar->addPermanentWidget(curlabel); //右侧开始

    connect(btnAuto, &QPushButton::clicked, m_camLightDlg, &camLightDlg::on_continuousmodel_clicked);
    connect(this, &MainWindow::lightValueSignal, m_camLightDlg, &camLightDlg::slotSetAllLightValue);
    connect(m_ImageForm, &ImageForm::RoiCreatedSignal, this, &MainWindow::slotStartAutoLight);
    connect(this, &MainWindow::lightDealEndSignal, m_ImageForm, &ImageForm::slot_lightend);
    connect(action_saveImage, &QAction::triggered, this, &MainWindow::on_action_saveImage_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int ret = QMessageBox::information(this, "温馨提示", "是否确认退出应用程序?", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        m_camLightDlg->closeDevice();
        QProcess::execute("taskkill /im Netcall-vxreg.exe /f");
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_action_set_triggered()
{
    //显示相机，光源设置
    m_camLightDlg->show();
    m_camLightDlg->raise();          //提升显示到所有父控件子类的顶部
    m_camLightDlg->activateWindow(); //设置顶部控件可活动
}

void MainWindow::grabImageUI(QImage image)
{
    m_image = image;
    //如果是ImageForm
    m_ImageForm->slot_updateVideo(image);
}

// static MainWindow *mainUI = nullptr;
void MainWindow::slotStartAutoLight(ImageForm::SRoi sroi)
{
    num      = 0;
    m_left   = sroi.left;
    m_top    = sroi.top;
    m_width  = sroi.width;
    m_height = sroi.height;

    int numoflights = 4;                    // 有几路光源
    int lower[4]    = {0, 0, 0, 0};         //如果4路光源，应该是 4个0（用于优化的最小值）
    int upper[4]    = {255, 255, 255, 255}; //如果4路光源，应该是 4个255（用于优化的最大值）
    int init[4]     = {128, 128, 128, 128}; //如果4路光源，建议是 4个128（用于优化的初值）
    int output[4]   = {0};                  //输出优化结果(4个整数)
    int population  = 10;                   //总体
    int numofgen    = 10;

    QString cbSearchNum = m_ComboBox->currentText();
    uint iSearchNum     = cbSearchNum.toUInt();
    qDebug() << "iSearchNum" << iSearchNum;
    switch (iSearchNum)
    {
    case 100:
        population = 10;
        numofgen   = 10;
        break;
    case 200:
        population = 20;
        numofgen   = 10;
        break;
    case 300:
        population = 30;
        numofgen   = 10;
        break;
    case 400:
        population = 20;
        numofgen   = 20;
        break;
    }
    maxlabel->setText("最优光源值:             评价值:    ");
    //    m_camLightDlg->resetLight(); //此处添加重新打开光源串口，不然设置光源值无效

    //初始化最大评价值
    m_MaxAppraise.value  = 0;
    m_MaxAppraise.lwhite = 0;
    m_MaxAppraise.lred   = 0;
    m_MaxAppraise.lgreen = 0;
    m_MaxAppraise.lblue  = 0;

    WaitDialog dlg(this, tr("处理中..."));
    dlg.show();
    curSeries->clear();
    QThread thread;
    bool ok = false;
    timer   = new QTimer();
    QObject::connect(
        &thread,
        &QThread::started,
        this,
        [&]() {
            OptMaxFunc maxfunc;
            maxfunc = MaxFunc;
            // qDebug() << "this 0x" << this;
            qDebug() << "m_left" << m_left << "m_top" << m_top;
            qDebug() << "m_width " << m_width << "m_height" << m_height;
            clock_t setstart = clock();
            vxSetOptimaizePara(this, numoflights, maxfunc);
            clock_t setend = clock();
            long setTime   = setend - setstart;
            qDebug() << "setTime " << setTime;

            clock_t optstart = clock();
            int vxret        = vxOptimaize(numoflights, lower, upper, init, output, population, numofgen, 8);
            ok               = true;
            clock_t optend   = clock();
            long optTime     = optend - optstart;
            qDebug() << "optTime " << optTime;
            qDebug() << "vxret " << vxret;
            appraiseDeal(output);
        },
        Qt::DirectConnection);
    thread.start();
    while (!ok) { qApp->processEvents(QEventLoop::ExcludeUserInputEvents); }
    thread.quit();
    thread.wait();
    m_camLightDlg->resetLight(); //此处添加重新打开光源串口，不然设置光源值无效
}

void MainWindow::appraiseDeal(int output[])
{
    qDebug() << "output" << output[0] << output[1] << output[2] << output[3];
    m_camLightDlg->slotSetAllLightValue(output[0], output[1], output[2], output[3], 13579);
    QThread::msleep(1200);
    zcRect roi;
    roi.Left   = m_left;
    roi.Top    = m_top;
    roi.Width  = m_width;
    roi.Height = m_height;
    qDebug() << "roi.Left" << roi.Left << "roi.Top" << roi.Top << "roi.Width" << roi.Width << "roi.Height" << roi.Height;
    QImage img = this->getImage();
    img.save("appraise.bmp");
    uchar *data = img.bits();
    int maxret  = ImageHistValue(data, img.width(), img.height(), roi);
    qDebug() << "max ret" << maxret;
    maxlabel->setText(QString::asprintf("最优光源值: %d %d %d %d 评价值: %d ", output[0], output[1], output[2], output[3], maxret));
    qDebug() << "m_MaxAppraise.value" << m_MaxAppraise.value << "m_MaxAppraise.lwhite lred lgreen lblue" << m_MaxAppraise.lwhite << m_MaxAppraise.lred << m_MaxAppraise.lgreen << m_MaxAppraise.lblue;
    emit lightDealEndSignal();
}

int MainWindow::MaxFunc(int numoflights, const int input[], void *optionrpara)
{
    //获取mainwindow
    MainWindow *mainUI = (MainWindow *)optionrpara;
    // qDebug() << "optionrpara 0x" << optionrpara;
    //获取roi的值
    zcRect roi;
    roi.Left   = mainUI->m_left;
    roi.Top    = mainUI->m_top;
    roi.Width  = mainUI->m_width;
    roi.Height = mainUI->m_height;
    //    qDebug() << "roi.Left" << roi.Left << "roi.Top" << roi.Top << "roi.Width" << roi.Width << "roi.Height" << roi.Height;
    qDebug() << "send light value" << input[0] << input[1] << input[2] << input[3] << "num =" << num;

    //发送光源设置信号
    emit mainUI->lightValueSignal(input[0], input[1], input[2], input[3], num);
    //    mainUI->lightlabel->setText(QString::asprintf("实时光源值: %d %d %d %d", input[0], input[1], input[2], input[3]));
    qDebug() << "send lightValueSignal";
    int lWhite = 0, lRed = 0, lGreen = 0, lBlue = 0;

    //读取配置文件的光源值，以此并不能判断光源是否设置成功，使用延时1s
    int n = 0;
    while (1)
    {
        QThread::msleep(1200);
        mainUI->m_camLightDlg->getLightValue(lWhite, lRed, lGreen, lBlue);
        qDebug() << "getL light value" << lWhite << lRed << lGreen << lBlue;
        if (input[0] == lWhite && input[1] == lRed && input[2] == lGreen && input[3] == lBlue) { break; }
        n++;
        if (n == 20)
        {
            qDebug() << "n == 20";
            break;
        }
    }

    //创建图像保存路径
    QString exePath = QApplication::applicationDirPath();
    QString imgPath = exePath + "/img";
    QDir fDir(imgPath);
    if (!fDir.exists()) fDir.mkdir(imgPath);

    //采集实时图像
    QImage image = mainUI->getImage();
    uchar *data  = image.bits();
    //    qDebug() << "img width" << image.width() << "img height" << image.height() << "roi.Width" << roi.Width << "roi.Height" << roi.Height;

    //评价函数，返回值为ROI区域的灰度差值
    int ret = ImageHistValue(data, image.width(), image.height(), roi);

    QString filePath =
        QString("%1/%2-%3-%4-%5-%6-%7_%8_%9_%10_%11.bmp").arg(imgPath).arg(num).arg(ret).arg(lWhite).arg(lRed).arg(lGreen).arg(lBlue).arg(roi.Left).arg(roi.Top).arg(roi.Width).arg(roi.Height);
    image.save(filePath);
    qDebug() << "ret" << ret;

    //    qDebug() << "m_MaxAppraise.value" << mainUI->m_MaxAppraise.value << "m_MaxAppraise.lwhite lred lgreen lblue" << mainUI->m_MaxAppraise.lwhite << mainUI->m_MaxAppraise.lred
    //             << mainUI->m_MaxAppraise.lgreen << mainUI->m_MaxAppraise.lblue;

    if (num == 0)
    {
        mainUI->m_MaxAppraise.value  = ret;
        mainUI->m_MaxAppraise.lwhite = lWhite;
        mainUI->m_MaxAppraise.lred   = lRed;
        mainUI->m_MaxAppraise.lgreen = lGreen;
        mainUI->m_MaxAppraise.lblue  = lBlue;
    }
    else if (mainUI->m_MaxAppraise.value <= ret) //如何评价返回值大于前面设置的最大评价值，设置此时的值为最大值
    {
        mainUI->m_MaxAppraise.value  = ret;
        mainUI->m_MaxAppraise.lwhite = lWhite;
        mainUI->m_MaxAppraise.lred   = lRed;
        mainUI->m_MaxAppraise.lgreen = lGreen;
        mainUI->m_MaxAppraise.lblue  = lBlue;
    }

    mainUI->curlabel->setText(QString::asprintf("当前评价值: %d ", ret));
    mainUI->curSeries->append(num, ret); //添加曲线点
    num++;

    return ret;
}

void MainWindow::createChart()
{
    //创建图表
    chartView = new QChartView(dockWidget);
    dockWidget->setWidget(chartView);
    chartView->setRubberBand(QChartView::RectangleRubberBand); //启用缩放

    QChart *chart = new QChart(); //创建chart
                                  //    chart->setTitle("评价值");
    chartView->setChart(chart);   //绑定

    //创建折线序列
    QLineSeries *series = new QLineSeries();
    series->setName("评价值折线");

    curSeries = series; //当前序列
    curSeries->clear();

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::red);
    series->setPen(pen); //折线序列的线条设置
    chart->addSeries(series);

    QValueAxis *axisX = new QValueAxis; // X轴
    curAxis           = axisX;          // 当前坐标轴
    axisX->setRange(0, 400);            //设置坐标轴范围
    axisX->setLabelFormat("%d");        //标签格式
    axisX->setTickCount(10);            //主要刻度
    axisX->setMinorTickCount(5);        //小刻度
    axisX->setTitleText("次数");        //标题

    QValueAxis *axisY = new QValueAxis; // Y轴
    axisY->setRange(0, 255);            //设置坐标轴范围
    axisY->setLabelFormat("%d");        //标签格式
    axisY->setTickCount(10);            //主要刻度
    axisY->setMinorTickCount(5);        //小刻度
    axisY->setTitleText("评价值");      //标题

    chart->setAxisX(axisX, series); //为序列设置坐标轴
    chart->setAxisY(axisY, series);
}

// int optfunc(int numoflights, const int input[], int optionrpara[])
//{
// return {-(input[0]*input[0]+input[1]*input[1]+ input[2] * input[2]+ input[3] * input[3]) };
// //T obj = -(x[0]* x[0] + x[1]*x[1]);
//}

//#include "optimaization.h"
// void test_opti()
//{
// int lower[] = { 0,0,0,0 };
// int upper[] = { 255,255,255,255 };
// int init[] = { 128,128,128,128 };
// int out[4];
// int optipara[] = { 0, 0, 0, 0 };

// vxSetOptimaizePara(4, optipara, 4, optfunc);
// vxOptimaize(4, lower, upper, init, out, 100,100,8);
//}

void MainWindow::on_action_saveImage_triggered()
{
    QImage image;
    image = m_ImageForm->getImageForm();
    if (image.isNull())
    {
        QMessageBox::warning(this, tr("警告"), tr("未采集到图像"));
        return;
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "/untitled.bmp", tr("Images (*.bmp)"));
        if (fileName.isEmpty()) return;
        if (image.save(fileName, "bmp")) QMessageBox::information(this, tr("ok"), tr("图像保存完毕！"));
    }
}
