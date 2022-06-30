#ifndef IMAGEFORM_H
#define IMAGEFORM_H

#include "toolrect.h"
//#include "toolroi.h"

#include "graphicspixmapitem.h"
//#include "measure/MeasureAssist.h"
//#include "optimaization.h"
#include <QGraphicsScene>
#include <QPushButton>
#include <QWidget>

namespace Ui
{
class ImageForm;
}

class ImageForm : public QWidget
{
    Q_OBJECT

public:
    explicit ImageForm(QWidget *parent = nullptr);
    ~ImageForm();
    QImage getImageForm() { return m_image; }
    //    int MaxFunc(int numoflights, const int input[], int optionrpara[]);

    struct SRoi
    {
        int top;
        int left;
        int width;
        int height;
    };

private:
    Ui::ImageForm *ui;
    QGraphicsScene *m_scene;          //场景
    GraphicsPixmapItem *m_pixmapItem; //图像
                                      //    ToolRoi *m_focusRoi;     //焦距ROI
    ToolRect *m_rect;                 //模板ROI
    QImage m_image;                   //当前图像,显示用

    bool m_first  = true;  //更新图来源后,确定显示缩放比时使用
    bool m_ok     = false; //算法内是否有图像
    bool m_source = 0;     //图像来源,0表示相机抓图,1表示图像文件

    int m_white;
    int m_red;
    int m_green;
    int m_blue;

    QPushButton *btnOk;
    QPushButton *btnCancel;

public slots:
    void slot_updateVideo(QImage image);
    void slot_lightend();

private slots:
    void slot_mouse_click(QPoint point);
    void slot_RoiCreate();

signals:
    void getLightSignal();
    void RoiCreatedSignal(SRoi sroi);
};

#endif // IMAGEFORM_H
