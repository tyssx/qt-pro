#include "WidgetTFunBtns.h"
#include "ui_WidgetTFunBtns.h"


#include <QDebug>

WidgetTFunBtns::WidgetTFunBtns(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetTFunBtns)
{
    ui->setupUi(this);

    //设置按钮边缘 按钮圆角
    //QPushButton{border:2px solid blue; border-top-left-radius:6px;border-top-right-radius:6px;border-bottom-left-radius:6px;border-bottom-right-radius:6px }

    //QButtonGroup *pBtnGroup= new QButtonGroup(this);
    pBtnGroup = new QButtonGroup(this);
    pBtnGroup->addButton(ui->PageBtn_Run,ePage_Run);
    pBtnGroup->addButton(ui->PageBtn_Pro,ePage_Proj);

    pBtnGroup->addButton(ui->PageBtn_Hardware,ePage_Hardware);
    pBtnGroup->addButton(ui->PageBtn_Image,ePage_ImgeAlgr);
    pBtnGroup->addButton(ui->PageBtn_Motion,ePage_MotionIO);
    pBtnGroup->addButton(ui->PageBtn_Data,ePage_DataList);
    pBtnGroup->addButton(ui->PageBtn_Mes,ePage_Mes);
    pBtnGroup->addButton(ui->PageBtn_Print,ePage_Print);
    pBtnGroup->addButton(ui->PageBtn_SysOption,ePage_SysOption);
    pBtnGroup->addButton(ui->PageBtn_Maintenance,ePage_Maintenance);

    //pBtnGroup->removeButton(ui->PageBtn_Motion);
    //ui->PageBtn_Motion->hide();
    //ui->PageBtn_Image->hide();

    //实现每次只能选一个 , 1 内个button设置 checkable属性  2 setExclusive(true);
    pBtnGroup->setExclusive(true);

    connect(pBtnGroup, SIGNAL(buttonClicked(int)),this,SIGNAL(activeTargetPage(int)));
    //connect(pBtnGroup, SIGNAL(buttonClicked(int)),this,SLOT(on_changeButtonGroup(int)));
}

WidgetTFunBtns::~WidgetTFunBtns()
{
    delete ui;
}

void WidgetTFunBtns::hideButton(int Id)
{
    QAbstractButton* btn = pBtnGroup->button(Id);
    pBtnGroup->removeButton(btn);

    //pBtnGroup->removeButton(ui->PageBtn_Motion);

    //ui->PageBtn_Motion->hide();

}

void WidgetTFunBtns::on_changeButtonGroup(int)
{
    // 额外的功能
    int n= pBtnGroup->checkedId();
    QPushButton *checkedButton = qobject_cast<QPushButton*>(pBtnGroup->checkedButton());
    QString checkedBtnObjectName = checkedButton->objectName();
    qDebug()<<tr("被选中按钮的objectName==%1").arg(checkedBtnObjectName);
    emit activeTargetPage(n);

}
