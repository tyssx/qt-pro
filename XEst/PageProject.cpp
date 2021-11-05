#include "PageProject.h"
#include "ui_PageProject.h"

PageProject::PageProject(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PageProject)
{
    ui->setupUi(this);
}

PageProject::~PageProject()
{
    delete ui;
}
