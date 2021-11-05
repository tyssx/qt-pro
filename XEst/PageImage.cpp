#include "PageImage.h"
#include "ui_PageImage.h"

PageImage::PageImage(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PageImage)
{
    ui->setupUi(this);
}

PageImage::~PageImage()
{
    delete ui;
}
