#include "widget.h"

#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    QStringList list;

    for (int i = 0; i < 16; i++)
    {
        QString list = tr("DO%1").arg(i, 2, 10, QLatin1Char('0'));
        ui->comboBox->appendItem(list, true);
    }
    //    ui->comboBox->setValues(list);
}

Widget::~Widget()
{
    delete ui;
}
