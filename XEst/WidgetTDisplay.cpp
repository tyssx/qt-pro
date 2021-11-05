#include "WidgetTDisplay.h"
#include "ui_WidgetTDisplay.h"

WidgetTDisplay::WidgetTDisplay(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::WidgetTDisplay)
{
    ui->setupUi(this);
}

WidgetTDisplay::~WidgetTDisplay()
{
    delete ui;
}
