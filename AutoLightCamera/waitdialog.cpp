#include "waitdialog.h"

WaitDialog::WaitDialog(QWidget *parent, const QString &title) : QDialog(parent)
{
    this->setFixedSize(100, 100);

    //设置透明度
    this->setWindowOpacity(0.8);

    //取消对话框标题
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->setStyleSheet("background-color:transparent;");

    //设置背景色为淡蓝色,
    background = new QFrame(this);
    background->setStyleSheet("background-color: rgb(55, 135,215);");
    background->setGeometry(0, 0, 100, 100);

    label = new QLabel(background);

    label->setStyleSheet("background-color:transparent;");

    label->setGeometry(30, 15, 40, 40);
    movie = new QMovie(":/images/wait3.gif");
    label->setScaledContents(true);
    label->setMovie(movie);

    tip_label = new QLabel(background);
    tip_label->setText(title);
    tip_label->setStyleSheet("color: white;background-color: transparent;");
    tip_label->setGeometry(10, 70, 80, 20);

    movie->start();
}

WaitDialog::~WaitDialog()
{
    delete label;
    delete movie;
    delete tip_label;
    delete background;
}
