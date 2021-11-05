#include "PageRun.h"
#include "ui_PageRun.h"

PageRun::PageRun(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PageRun)
{
    ui->setupUi(this);
}

PageRun::~PageRun()
{
    delete ui;
}
