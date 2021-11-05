#include "WidgetTabInfo.h"
#include "ui_WidgetTabInfo.h"

WidgetTabInfo::WidgetTabInfo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetTabInfo)
{
    ui->setupUi(this);
}

WidgetTabInfo::~WidgetTabInfo()
{
    delete ui;
}
