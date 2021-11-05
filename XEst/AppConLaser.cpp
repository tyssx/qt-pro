#include "AppConLaser.h"

//==================================
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "WidgetTDisplay.h"
#include "WidgetTFunBtns.h"
#include "WidgetTabInfo.h"

#include "PageRun.h"
#include "PageProject.h"
#include "PageImage.h"

AppConLaser::AppConLaser(QObject *parent) : QObject(parent)
{

}

AppConLaser::AppConLaser(QWidget *w, QObject *parent ) : QObject(parent)
{
    pMain = (MainWindow*)w;
    //pMain = dynamic_cast<MainWindow*>(w);
}

AppConLaser::~AppConLaser()
{

}


void AppConLaser::setMainWindow(MainWindow *w)
{
    if(nullptr == w) return;
    pMain =  w;
}

bool AppConLaser::createUi()
{

    if(nullptr == pMain) return false;

    //============================================
    groupConLaserUi.pUiTDisplay  = new WidgetTDisplay(pMain);
    groupConLaserUi.pUiTFunBtns  = new WidgetTFunBtns(pMain);

    groupConLaserUi.pUiTabInfo  =new WidgetTabInfo(pMain);

    //创建每个页面
    groupConLaserUi.pUiPageRun      = new PageRun(pMain);
    groupConLaserUi.pUiPageProject  = new PageProject(pMain);
    groupConLaserUi.pUiPageImage    = new PageImage(pMain);

    //加入 显示界面
    addUiTDisplay(groupConLaserUi.pUiTDisplay);
    //pMain->addUiTDisplay(groupConLaserUi.pUiTDisplay);
    pMain->addUiTabInfo(groupConLaserUi.pUiTabInfo);

    // 先把各页面 加入
    pMain->addUi_StackedPage(groupConLaserUi.pUiPageRun);
    pMain->addUi_StackedPage(groupConLaserUi.pUiPageProject);
    pMain->addUi_StackedPage(groupConLaserUi.pUiPageImage);
    //
    pMain->addUiTFunBtns(groupConLaserUi.pUiTFunBtns);

    //
    pMain->setUi_CurStPage(groupConLaserUi.pUiPageRun);

    // 设置第0列 和第1列的比例为 1:1
    //ui->horLayoutMain->setStretch(0,1);
    //ui->horLayoutMain->setStretch(1,1);
    //ui->horLayoutMain->setSpacing(1);

     connect(groupConLaserUi.pUiTFunBtns, SIGNAL(activeTargetPage(int)), this, SLOT(changePage(int)));

     return true;
}

void AppConLaser::addUiTDisplay(QWidget *w)
{
  pMain->getMainUi()->verLayoutDisplay->addWidget(w);
  //pMain->addUiTDisplay(w);
}

void AppConLaser::addUiTabInfo(QWidget *w)
{
    pMain->getMainUi()->horLayoutTabInfo->addWidget(w);
    //pMain->addUiTabInfo(w);
}

void AppConLaser::addUiTFunBtns(QWidget *w)
{
    pMain->getMainUi()->verLayoutBtns->addWidget(w);
    //pMain->addUiTabInfo(w);
}

void AppConLaser::addUiTFunPages(QWidget *w)
{
    pMain->getMainUi()->VesTFunStPage->addWidget(w);
    //pMain->addUiTFunPages(w);

}

void AppConLaser::setUiTFunCurPage(QWidget *w)
{
    pMain->getMainUi()->VesTFunStPage->setCurrentWidget(w);
    //pMain->setUiTFunCurPage(w);
}



void AppConLaser::showUiTFunBtns(bool bShow)
{
    if(bShow)
    {
        pMain->getMainUi()->VesTFunBtns->show();
    }

    else
    {
        pMain->getMainUi()->VesTFunBtns->hide();
    }
}

void AppConLaser::showUiTDisplay(bool bShow)
{
    if(bShow)
    {
        pMain->getMainUi()->VesTDisplay->show();
    }

    else
    {
        pMain->getMainUi()->VesTDisplay->hide();
    }
}

void AppConLaser::showUiTabInfo(bool bShow)
{
    if(bShow)
    {
        pMain->getMainUi()->VesTabInfo->show();
    }

    else
    {
        pMain->getMainUi()->VesTabInfo->hide();
    }
}

void AppConLaser::changePage(int nActivePage)
{
    switch (nActivePage)
    {
    case ePage_Run:
        pMain->setUi_CurStPage(groupConLaserUi.pUiPageRun);
        pMain->getMainUi()->labStPage->setText(QString("运行页"));
        showUiTFunBtns(false);
        break;
    case ePage_Proj:
        pMain->setUi_CurStPage(groupConLaserUi.pUiPageProject);
        pMain->getMainUi()->labStPage->setText(QString("工程文件页"));
        break;
    case ePage_ImgeAlgr:

        pMain->setUi_CurStPage(groupConLaserUi.pUiPageImage);
        pMain->getMainUi()->labStPage->setText(QString("图像页"));
        break;
    case ePage_Hardware:

        //showUiTDisplay(false);
        //showUiTabInfo(false);
        //showUiTFunBtns(false);
       break;


    default: break;
    }
}


