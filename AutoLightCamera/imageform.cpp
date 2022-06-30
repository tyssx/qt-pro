#include "imageform.h"

#include "camlightdlg.h"
#include "ui_imageform.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QString>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

// int MaxFunc(int numoflights, const int input[], int optionrpara[]);

ImageForm::ImageForm(QWidget *parent) : QWidget(parent), ui(new Ui::ImageForm)
{
    ui->setupUi(this);

    //场景
    m_scene = new QGraphicsScene;
    ui->graphicsView->setScene(m_scene);

    //图像及动态显示
    m_pixmapItem = new GraphicsPixmapItem;
    m_scene->addItem(m_pixmapItem); //添加到场景
    m_pixmapItem->setZValue(-2);    //叠放顺序

    // roi工具
    m_rect = new ToolRect;
    m_scene->addItem(m_rect);
    m_first = true;
#if 1
    QImage image;
    //    image.load("E:\\Qt project\\project\\AutoLightCamera\\image\\test.bmp");
    image.load("D:\\softwareData\\qt project\\project\\build-AutoLightCamera-Desktop_Qt_5_12_1_MSVC2017_64bit-Release\\release\\img\\67-14-224-242-241-247_1807_2301_104_61.bmp");
    m_pixmapItem->setPixmap(QPixmap::fromImage(image));
    m_image = image;
#endif
    //信号与槽
    //检测到view中的鼠标按下，执行槽函数
    connect(ui->graphicsView, &GraphicsView::mouseClicked, this, &ImageForm::slot_mouse_click);
    //当鼠标松开，发送detectChanged信号，执行槽函数
    connect(m_rect, &ToolRect::detectChanged, this, &ImageForm::slot_RoiCreate);
}

ImageForm::~ImageForm()
{
    delete ui;
}

void ImageForm::slot_mouse_click(QPoint point)
{
    qDebug() << "slot_mouse_click";
    QPointF pos = ui->graphicsView->mapToScene(point); //转换到scene坐标
    qDebug() << pos.rx() << pos.ry();

    m_rect->setVisible(true);
    m_rect->reset();
    m_rect->setPos(pos);
}

void ImageForm::slot_RoiCreate()
{
    qDebug() << "slot_templateCreate";
    if (m_image.isNull())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("当前图像为空无法创建模板！"));
        m_rect->setVisible(false);
        return;
    }

    //检测roi是否与图像有交集
    QPolygonF pg = m_rect->getPolygon();
    pg.translate(m_rect->pos().x(), m_rect->pos().y());

    if (!pg.intersects(QPolygon(m_image.rect())))
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("选择框在图像外！"));
        m_rect->setVisible(false);
        return;
    };
    qDebug() << "roi width " << m_rect->width();
    qDebug() << "roi height " << m_rect->height();
    qDebug() << "roi x " << m_rect->pos().x() << "roi y " << m_rect->pos().y();
    qDebug() << "img x " << m_image.rect().x() << "img y " << m_image.rect().y();
    SRoi sroi;
    sroi.left   = m_rect->pos().x();
    sroi.top    = m_rect->pos().y();
    sroi.width  = m_rect->width();
    sroi.height = m_rect->height();

    emit RoiCreatedSignal(sroi);
    qDebug() << "emit RoiCreatedSignal";
}

void ImageForm::slot_updateVideo(QImage image)
{
    m_image = image;
    m_pixmapItem->setPixmap(QPixmap::fromImage(image));
    //第一次显示时确定最合适比例
    if (m_first)
    {
        m_first = false;
        m_scene->setSceneRect(m_pixmapItem->boundingRect());
        ui->graphicsView->fitInView(m_pixmapItem, Qt::KeepAspectRatio);
    }
}

void ImageForm::slot_lightend()
{
    // m_rect->clearRect();
    m_rect->setVisible(false);
}
