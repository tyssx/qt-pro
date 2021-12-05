#include "colorsetdialog.h"

#include "ui_colorsetdialog.h"

ColorSetDialog::ColorSetDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ColorSetDialog)
{
    ui->setupUi(this);

    ui->horizontalSliderAlpha->setValue(255);
    ui->radioButtonDec->setChecked(true);

    connect(ui->horizontalSliderRed, &QSlider::valueChanged, this, &ColorSetDialog::slot_colorSliderChanged);
    connect(ui->horizontalSliderGreen, &QSlider::valueChanged, this, &ColorSetDialog::slot_colorSliderChanged);
    connect(ui->horizontalSliderBlue, &QSlider::valueChanged, this, &ColorSetDialog::slot_colorSliderChanged);
    connect(ui->horizontalSliderAlpha, &QSlider::valueChanged, this, &ColorSetDialog::slot_colorSliderChanged);

    connect(ui->radioButtonDec, &QRadioButton::clicked, this, &ColorSetDialog::slot_radioButtonClicked);
    connect(ui->radioButtonBin, &QRadioButton::clicked, this, &ColorSetDialog::slot_radioButtonClicked);
    connect(ui->radioButtonOct, &QRadioButton::clicked, this, &ColorSetDialog::slot_radioButtonClicked);
    connect(ui->radioButtonHex, &QRadioButton::clicked, this, &ColorSetDialog::slot_radioButtonClicked);
}

ColorSetDialog::~ColorSetDialog()
{
    delete ui;
}

void ColorSetDialog::slot_colorSliderChanged()
{
    QColor color;
    int rV = ui->horizontalSliderRed->value();
    int gV = ui->horizontalSliderGreen->value();
    int bV = ui->horizontalSliderBlue->value();
    int aV = ui->horizontalSliderAlpha->value();
    color.setRgb(rV, gV, bV, aV); //设置颜色

    QPalette palet = ui->textEditColor->palette();
    palet.setColor(QPalette::Base, color); //设置背景色
    ui->textEditColor->setPalette(palet);
}

void ColorSetDialog::slot_radioButtonClicked()
{
    QRadioButton *radioBtn = qobject_cast<QRadioButton *>(sender());
    if (radioBtn == ui->radioButtonDec)
    {
        //显示十进制
        ui->lcdNumber->setDigitCount(3); //设置位数
        ui->lcdNumber->setDecMode();
    }
    else if (radioBtn == ui->radioButtonBin)
    {
        //显示二进制
        ui->lcdNumber->setDigitCount(8); //设置位数
        ui->lcdNumber->setBinMode();
    }
    if (radioBtn == ui->radioButtonOct)
    {
        //显示八进制
        ui->lcdNumber->setDigitCount(4); //设置位数
        ui->lcdNumber->setOctMode();
    }
    if (radioBtn == ui->radioButtonHex)
    {
        //显示十六进制
        ui->lcdNumber->setDigitCount(3); //设置位数
        ui->lcdNumber->setHexMode();
    }
}

void ColorSetDialog::on_dial_valueChanged(int value)
{
    ui->lcdNumber->display(value); //设置LCD显示
}

void ColorSetDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->horizontalScrollBar->setValue(value);
    ui->progressBarHor->setValue(value);
}

void ColorSetDialog::on_horizontalScrollBar_valueChanged(int value)
{
    ui->horizontalSlider->setValue(value);
    ui->progressBarHor->setValue(value);
}

void ColorSetDialog::on_verticalSlider_valueChanged(int value)
{
    ui->verticalScrollBar->setValue(value);
    ui->progressBarVer->setValue(value);
}

void ColorSetDialog::on_verticalScrollBar_valueChanged(int value)
{
    ui->verticalSlider->setValue(value);
    ui->progressBarVer->setValue(value);
}
