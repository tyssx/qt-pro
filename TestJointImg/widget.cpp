#include "widget.h"

#include "ui_widget.h"

#include <QDebug>
#include <QPainter>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    QImage qimg1, qimg2;
    //    qimg1.load("F:\\资料\\图像\\1.bmp");
    //    qimg2.load("F:\\资料\\图像\\2.bmp");
    qimg1.load("F:\\资料\\图像\\邦正\\1.bmp");
    qimg2.load("F:\\资料\\图像\\邦正\\2.bmp");
    int h1 = 51; //第一次走料高度
    int h2 = 51; //第二次走料高度
    //像素当量
    double pixPerMM = 798 / 51.0;
    qDebug() << "pixPerMM" << pixPerMM;

#if 1 //竖向
    //    QImage qimg3  = QImage(qimg1.width(), qimg1.height(), QImage::Format_Grayscale8);
    //    QPainter p(&qimg3);
    //    p.drawPixmap(0, 0, qimg1.width(), qimg1.height(), QPixmap::fromImage(qimg1));
    //    p.drawPixmap(0, qimg1.height(), qimg2.width(), qimg2.height(), QPixmap::fromImage(qimg2));
    //    p.drawPixmap(
    //        0, 0, qimg1.width(), 798, QPixmap::fromImage(qimg1.copy(0, 0, qimg1.width(), 798)));
    //    p.drawPixmap(
    //        0, 798, qimg2.width(), 798, QPixmap::fromImage(qimg2.copy(0, 0, qimg1.width(), 798)));
    QImage qcopy1 = qimg1.copy(0, 0, qimg1.width(), h1 * pixPerMM);
    QImage qcopy2 = qimg2.copy(0, 0, qimg2.width(), h2 * pixPerMM);
    QImage qimg3  = QImage(qimg1.width(), h1 * pixPerMM + h2 * pixPerMM, QImage::Format_Grayscale8);
    QPainter p(&qimg3);
    p.drawPixmap(0, 0, qcopy1.width(), qcopy1.height(), QPixmap::fromImage(qcopy1));
    p.drawPixmap(0, qcopy1.height(), qcopy2.width(), qcopy2.height(), QPixmap::fromImage(qcopy2));
#else //横向
    QImage qimg3 = QImage(qimg1.width() + qimg2.width(), qimg1.height(), QImage::Format_Grayscale8);
    QPainter p(&qimg3);
    p.drawPixmap(0, 0, qimg1.width(), qimg1.height(), QPixmap::fromImage(qimg1));
    p.drawPixmap(qimg1.width(), 0, qimg2.width(), qimg2.height(), QPixmap::fromImage(qimg2));
#endif
    QPixmap pix3 = QPixmap::fromImage(qimg3);
    qimg3.save("F:\\资料\\图像\\邦正\\3.bmp");
    ui->label->setPixmap(pix3.scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
}

Widget::~Widget()
{
    delete ui;
}
