#include "widget.h"

#include "ui_widget.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

cv::Mat PureQImage2cvMat(const QImage &image)
{
    cv::Mat mat;
    qDebug() << "image.format" << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(),
            image.width(),
            CV_8UC4,
            (void *)image.constBits(),
            image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(),
            image.width(),
            CV_8UC3,
            (void *)image.constBits(),
            image.bytesPerLine());
        break;
    case QImage::Format_Indexed8:
    case QImage::Format_Grayscale8:
        mat = cv::Mat(
            image.height(), image.width(), CV_8UC1, (void *)image.bits(), image.bytesPerLine());
        break;
    }
    return mat;
}

QImage cvMat2QImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for (int i = 0; i < 256; i++) { image.setColor(i, qRgb(i, i, i)); }
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    else if (mat.type() == CV_8UC3)
    {
        const uchar *pSrc = (const uchar *)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if (mat.type() == CV_8UC4)
    {
        const uchar *pSrc = (const uchar *)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, (int)mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        return QImage();
    }
}

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    //场景
    m_scene1 = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene1);

    //图像及动态显示
    m_pixmapItem1 = new QGraphicsPixmapItem;
    m_scene1->addItem(m_pixmapItem1);
    m_pixmapItem1->setZValue(-2);

    m_scene2 = new QGraphicsScene(this);
    ui->graphicsView_2->setScene(m_scene2);

    //图像及动态显示
    m_pixmapItem2 = new QGraphicsPixmapItem;
    m_scene2->addItem(m_pixmapItem2);
    m_pixmapItem2->setZValue(-2);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButtonLoad_clicked()
{
    QString impath = QFileDialog::getOpenFileName(this,
        "读取图像对话框",
        QApplication::applicationDirPath(),
        tr("Image File(*.jpg *.png *.bmp)"));
    if (impath.isNull())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("图像加载失败!"));
        return;
    }
    QImage image;
    if (image.load(impath))
    {
        m_image = image;
        qDebug() << "m_image = image";
        m_pixmapItem1->setPixmap(QPixmap::fromImage(image));
        ui->graphicsView->fitInView(m_pixmapItem1, Qt::KeepAspectRatio); //按最小比例显示
        qDebug() << "sendImage";
    }
}

#if 0
void Widget::on_pushButtonToINT64_clicked()
{
    if (m_image.isNull())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("图像未加载!"));
        return;
    }
    cv::Mat colorMat = PureQImage2cvMat(m_image);
    IplImage ipl(colorMat);

    INT64 handle;
    vxPopReleaseImage(handle); //释放内存空间
    qDebug() << "release";
    if (ui->lineEdit->text().isEmpty()) { handle = vxPutImage(&ipl); }
    else
    {
        char *format = ui->lineEdit->text().toLocal8Bit().data();
        qDebug() << "format" << format;
        handle = vxPutImage(&ipl, format);
    }

    qDebug() << "handle is" << handle;
    ui->textEdit->append(QString::asprintf("image depth is %d", m_image.depth()));
    ui->textEdit->append(QString::asprintf("hand is %lld", handle));

    IplImage *tranIpl = vxGetIplimage(handle);
    cv::Mat tranMat   = cv::cvarrToMat(tranIpl);
    qDebug() << "tranMat" << tranMat.type();
    remove("handle.bmp");
    cv::imwrite("handle.bmp", tranMat);
    QImage image = cvMat2QImage(tranMat);
    m_pixmapItem2->setPixmap(QPixmap::fromImage(image));
    ui->graphicsView_2->fitInView(m_pixmapItem2, Qt::KeepAspectRatio); //按最小比例显示
    ui->textEdit->append("show transform Image");
}
#else
void Widget::on_pushButtonToINT64_clicked()
{
    if (m_image.isNull())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("图像未加载!"));
        return;
    }
    //    cv::Mat colorMat = PureQImage2cvMat(m_image);
    //    IplImage ipl(colorMat);

    //    m_image = m_image.convertToFormat(QImage::Format_RGB888);
    qDebug() << "on_pushButtonToINT64_clicked image format" << m_image.format();

    INT64 handle;
    vxPopReleaseImage(handle); //释放内存空间
    qDebug() << "release";
    if (ui->lineEdit->text().isEmpty()) { return; }
    else
    {
        char *format = ui->lineEdit->text().toLocal8Bit().data();
        qDebug() << "format" << format;
        handle = vxPutImageRawData(m_image.bits(), format, m_image.width(), m_image.height(), 8);
    }

    //    char format[5] = "BGR";

    qDebug() << "handle is" << handle;
    ui->textEdit->append(QString::asprintf("image depth is %d", m_image.depth()));
    ui->textEdit->append(QString::asprintf("hand is %lld", handle));

    IplImage *tranIpl = vxGetIplimage(handle);
    cv::Mat tranMat   = cv::cvarrToMat(tranIpl);
    qDebug() << "tranMat" << tranMat.type();
    remove("handle.bmp");
    cv::imwrite("handle.bmp", tranMat);
    QImage image = cvMat2QImage(tranMat);
    m_pixmapItem2->setPixmap(QPixmap::fromImage(image));
    ui->graphicsView_2->fitInView(m_pixmapItem2, Qt::KeepAspectRatio); //按最小比例显示
    ui->textEdit->append("show transform Image");
}
#endif

void Widget::on_pushButtonClear_clicked()
{
    ui->textEdit->clear();
}
