#ifndef EXTVARIATION3CHITEM_H
#define EXTVARIATION3CHITEM_H

//#include "esviitem.h"
#include "esvidataitem.h"
#include "extvariation3chtask.h"

#include <QGraphicsObject>

class Variation3ChPage;
class Widget;

class ExtVariation3ChItem : public QGraphicsObject
{
    Q_OBJECT

public:
    // data数据定义
    enum
    {
        TASK_NAME,   //任务名称
        TASK_POINTER //关联的任务指针，后续可优化去除
    };

    //类型
    enum
    {
        EsviItemType = QGraphicsItem::UserType + 10000
    };
    //建立过程状态
    enum State
    {
        Init    = 0x01, //初始状态值
        Ongoing = 0x02, //进行中
        Success = 0x04, //建立成功
        Failed  = 0x08  //建立失败
    };
    Q_INVOKABLE ExtVariation3ChItem();
    ~ExtVariation3ChItem();

    enum
    {
        Type = EsviItemType + kExtVariation3Ch
    };
    int type() const override { return Type; }
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPainterPath shape() const override;
    //    void setDefaultName();
    void editorDetect();
    //    QWidget *property() override;
    void buildFromCtrlpoint(QPointF pt, /*DataUnit *du,*/ bool isTrack);

    //显示检测结果
    void updateOut();

    void createShape(bool calWidth = true);
    //    void progressDetect(); //带进度条的检测过程
    const State &buildState() { return m_buildState; }
    bool checkIn() { return true; } //检测in的有效性
    void setCurrent(bool hideResult = true);
    void setImgHandle(INT64 handle, Widget *widget = nullptr);
    ExtVariation3ChTask::Param m_param;
    AbstractDisplay *m_display = nullptr; //检测结果的显示输出，子类可自行定义

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    State m_buildState = Init; //交互建立过程中的状态

signals:
    void sendEdges(CxPointArray roiArray, CxPointArray segment, CxPointArray CenterArea);

signals:
    void detectChanged(QGraphicsItem *);

private:
    ExtVariation3ChTask *m_task;
    EsviDataItem m_result;

    Widget *m_widget;
    int m_direction;          //变形模式
    bool m_transform = false; //变形标志
    QPointF m_start;          //开始点
    bool m_hover   = false;   //掠过标志
    qreal m_factor = 1;       //因数

    /////////////////////////////////////
    qreal m_width = 0;             //宽度
    QLineF m_line{{0, 0}, {0, 0}}; //指示线
    QPolygonF m_polygon;           //外框
    QPainterPath m_shape;          //拾取形状

    enum
    {
        Num = 3
    };
    QPointF m_ctrl[Num]; //控制点，两个端点，第三个为方向控制点
    int m_count = 0;     //有效控制点个数

    Variation3ChPage *m_property;

    ExtVariation3ChTask::SimpParam m_simpParam;

    INT64 m_modelId = -1; //模板id
};

#endif // EXTVARIATION3CHITEM_H
