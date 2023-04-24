#include "widget.h"

#include "extpolygonitem.h"
#include "extvariation3chitem.h"
#include "graphicsscene.h"
#include "ui_widget.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>

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

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->comboBoxChannel->setCurrentIndex(3);
    //    ui->pushButtonToINT64->setEnabled(false);
    //场景
    m_scene = new GraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    //图像及动态显示
    m_pixmapItem = new QGraphicsPixmapItem;
    m_scene->addItem(m_pixmapItem);
    m_pixmapItem->setZValue(-2);

    m_item        = new ExtVariation3ChItem;
    m_Polygonitem = new ExtPolygonitem;
    //    m_scene->addItem(m_Polygonitem);
    connect(m_scene, &GraphicsScene::result, this, &Widget::slot_addItem);
    connect(ui->pushButtonApply, &QPushButton::clicked, this, &Widget::slot_apply);
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
        m_pixmapItem->setPixmap(QPixmap::fromImage(image));
        ui->graphicsView->fitInView(m_pixmapItem, Qt::KeepAspectRatio); //按最小比例显示
        qDebug() << "sendImage";

        char format[5] = "BGRA";   // OpenCV图像数据格式
        vxPopReleaseImage(handle); //释放内存空间

        handle = vxPutImageRawData(image.bits(),
            format,
            image.width(),
            image.height(),
            8); //直接将QImage转为算法需要的INT64的句柄，省去中间转OpenCV

        m_scene->setCurTool(1);
    }
}

void Widget::on_pushButtonToINT64_clicked()
{
    //    if (m_image.isNull())
    //    {
    //        QMessageBox::warning(this, tr("温馨提示"), tr("图像未加载!"));
    //        return;
    //    }
    //    cv::Mat colorMat = PureQImage2cvMat(m_image);
    //    IplImage ipl(colorMat);

    //    INT64 handle;
    //    vxPopReleaseImage(handle); //释放内存空间
    //    qDebug() << "release";
    //    handle = vxPutImage(&ipl);
    //    qDebug() << "handle is" << handle;
    //    ui->textEdit->append(QString::asprintf("image depth is %d", m_image.depth()));
    //    ui->textEdit->append(QString::asprintf("hand is %lld", handle));
}

void Widget::on_pushButtonClear_clicked()
{
    ui->textEdit->clear();
}

void Widget::slot_addItem(ExtVariation3ChItem *item)
{
    m_item = item;
    m_item->setImgHandle(handle, this);
    m_item->setVisible(true);
    m_item->setSelected(true);
    m_item->editorDetect();
    //    m_item->update();
    connect(m_item,
        SIGNAL(detectChanged(QGraphicsItem *)),
        this,
        SLOT(slot_esviItem_update(QGraphicsItem *)));
    slot_esviItem_update(m_item);
}

void Widget::slot_apply()
{
    m_item->m_param.segma   = ui->doubleSpinBoxSegma->value();
    m_item->m_param.erosion = ui->doubleSpinBoxErosion->value();
    m_item->m_param.channel = ui->comboBoxChannel->currentText().toInt();
    m_item->editorDetect();
    emit m_item->detectChanged(m_item);
}

void Widget::slot_esviItem_update(QGraphicsItem *cur)
{
    m_scene->blockSignals(true);
    m_scene->clearSelection();
    m_scene->blockSignals(false);
    cur->setSelected(true);

    qDebug() << "item在场景中的检测结果显示刷新";
    m_item->updateOut();
}

void Widget::slot_getEdges(CxPointArray roiArray, CxPointArray segment, CxPointArray CenterArea)
{
    qDebug() << "get edges";
    ui->textEdit->append(
        QString::asprintf("roi size is %d coord=\n (%lf,%lf)-(%lf,%lf)-(%lf,%lf)-(%lf,%lf)",
            roiArray.GetSize(),
            roiArray[0].GetX(),
            roiArray[0].GetY(),
            roiArray[1].GetX(),
            roiArray[1].GetY(),
            roiArray[2].GetX(),
            roiArray[2].GetY(),
            roiArray[3].GetX(),
            roiArray[3].GetY()));
    ui->textEdit->append(QString::asprintf("segment size is %d", segment.GetSize()));
    ui->textEdit->append(QString::asprintf("CenterArea size is %d", CenterArea.GetSize()));
}
