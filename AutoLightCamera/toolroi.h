#ifndef TOOLROI_H
#define TOOLROI_H

#include <QGraphicsObject>
//此值在esvidataitem.h
const int kPickSize = 15; //拾取精度/控制点大小
class ToolRoi : public QGraphicsObject
{
    Q_OBJECT
public:
    ///
    /// \brief ToolRoi 构造ROI工具
    ///
    ToolRoi();
    ~ToolRoi();

    ///
    /// \brief boundingRect QGraphicsItem类的绘图边界
    /// \return 边界范围
    ///
    QRectF boundingRect() const;

    ///
    /// \brief paint 绘制项目的内容
    /// \param painter 绘图板
    /// \param option 绘图选项
    /// \param widget 正在绘制的小部件
    ///
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    ///
    /// \brief shape 获取工具的形状
    /// \return 形状
    ///
    QPainterPath shape() const;

    ///
    /// \brief rectFeatures 计算旋转矩形的特征值
    /// \param row 中心点行坐标
    /// \param col 中心点列坐标
    /// \param phi 角度，Y轴向上弧度值
    /// \param len1 长轴
    /// \param line2 短轴
    ///
    void rectFeatures(double &row, double &col, double &phi, double &len1, double &line2);

    ///
    /// \brief getPolygonF 获取工具的多边形数据
    /// \return 多边形数据
    ///
    QPolygonF getPolygonF();

    ///
    /// \brief reset 重置所有设置
    ///
    void reset();

    ///
    /// \brief createShape 建立工具形状
    ///
    void createShape();

    ///
    /// \brief setRect 重置工具框为空
    ///
    void setRect(QRect = QRect());

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    qreal m_width;       //框宽度
    QLineF m_line;       //框基线
    QPolygonF m_polygon; //外观

    int m_direction;  //变形模式
    bool m_transform; //变形标志
    QPointF m_start;  //开始点
    QPointF m_end;    //结束点

    bool m_hover;   //掠过标志
    qreal m_factor; //因数

    bool m_first; //首次拉变

signals:
    ///
    /// \brief toolChange 信号: 工具框发生改变
    ///
    void toolChange();
};

#endif // TOOLROI_H
