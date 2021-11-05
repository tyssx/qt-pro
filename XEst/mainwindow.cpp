#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>

//
#include "AppConLaser.h"


//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    readUiPosSettings();

    ui->setupUi(this);
    setWindowTitle("XEst"); //修改主窗口标题

    // 创建界面
    pLaser = new AppConLaser(this);
    pLaser->createUi();

    //pSensor = new AppSensor(this);
    //pSensor->createUi();


    //ui->horLayoutMain->setStretch(0,0);

}

MainWindow::~MainWindow()
{
    writeUiPosSettings();

    delete ui;
}

Ui::MainWindow * MainWindow::getMainUi()
{
    return ui;
}

void MainWindow::addUiTDisplay(QWidget *pWidget)
{
   ui->verLayoutDisplay->addWidget(pWidget);
}


void MainWindow::addUiTFunBtns(QWidget *pWidget)
{
    ui->verLayoutBtns->addWidget(pWidget);
}

void MainWindow::addUiTabInfo(QWidget *pWidget)
{
    ui->horLayoutTabInfo->addWidget(pWidget);
}

void MainWindow::addUi_StackedPage(QWidget *w)
{
   ui->VesTFunStPage->addWidget(w);
}

void MainWindow::setUi_CurStPage(QWidget *w)
{
    ui->VesTFunStPage->setCurrentWidget(w);
}

void MainWindow::showUiTFunBtns(bool bShow)
{
    if(bShow)
        this->ui->VesTFunBtns->show();
    else
        this->ui->VesTFunBtns->hide();
}

void MainWindow::showUiTDisplay(bool bShow)
{
    if(bShow)
     ui->horLayoutMain->setStretch(0,1);
    else
     ui->horLayoutMain->setStretch(0,0);
}

void MainWindow::showUiTabInfo(bool bShow)
{
    if(bShow)
     ui->horLayoutMain->setStretch(0,1);
    else
     ui->horLayoutMain->setStretch(0,0);
}




void MainWindow::writeUiPosSettings()
{
    QSettings settings("SoftwareXEst","UiPos");
    settings.beginGroup("mainWindow");
    settings.setValue("size", size());
    settings.setValue("state", saveState());
    settings.endGroup();

}

void MainWindow::readUiPosSettings()
{
    QSettings settings("SoftwareXEst","UiPos");
    settings.beginGroup("mainWindow");
    resize(settings.value("size").toSize());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();

}

void MainWindow::on_actionEditModel_triggered()
{
    showUiTFunBtns(true);
}
