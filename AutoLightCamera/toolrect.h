#ifndef TOOLRECT_H
#define TOOLRECT_H

#include <QGraphicsObject>
const int kRectItem = QGraphicsItem::UserType + 01000;
// extern const int kPickSize = 15; //拾取精度/控制点大小
class ToolRect : public QGraphicsObject
{
    Q_OBJECT

    ///
    /// \brief operator <<友元，流写入
    /// \param out 输出流
    /// \param item 旋转矩形工具
    /// \return 输出流
    ///
    //    friend QDataStream &operator<<(QDataStream &out, const ToolRect &item);

    ///
    /// \brief operator >>友元，流读出
    /// \param in 输入流
    /// \param item 旋转矩形工具
    /// \return 输入流
    ///
    //    friend QDataStream &operator>>(QDataStream &in, ToolRect &itemt);

public:
    ///
    /// \brief ToolRect 构造旋转矩形
    ///
    ToolRect();
    ~ToolRect();

    enum
    {
        Type = kRectItem
    };

    ///
    /// \brief type 获取工具类型
    /// \return 工具类型
    ///
    int type() const { return Type; }

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
    /// \brief rectFeatures 计算旋转矩形的特征值
    /// \param row 中心点行坐标
    /// \param col 中心点列坐标
    /// \param phi 角度，逆时针度数，
    /// \param len1 长轴
    /// \param line2 短轴
    ///
    void rectFeatures(double &row, double &col, double &phi, double &len1, double &line2);
    QRectF getQRectF();
    //    CvRect mapToCv(); //转成cv相关格式

    ///
    /// \brief reset 重置所有设置
    ///
    void reset();

    ///
    /// \brief getPolygonF 获取工具的多边形数据
    /// \return 多边形数据
    ///
    const QPolygonF &getPolygon();

    ///
    /// \brief setCanEdit 设置工具能否编辑
    ///
    void setCanEdit(bool);

    ///
    /// \brief createShape 建立工具形状
    ///
    void createShape();

    //配合protobuf开放接口
    ///
    /// \brief line 获取工具的基线
    /// \return 基线
    ///
    QLineF &line() { return m_line; }

    ///
    /// \brief width 获取工具的宽度
    /// \return 宽度
    ///
    qreal &width() { return m_width; }
    qreal &height() { return m_height; }

    qreal ptToLine(const QLineF &line, const QPointF &pt);

    //新增
    void clearRect();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private:
    qreal m_width;       //框宽度
    qreal m_height;      //框高度
    QLineF m_line;       //框基线
    QPolygonF m_polygon; //外观，即绘制的矩形
    bool m_canEdit;      // ROI确定后，不充许编辑

    int m_direction;  //变形模式
    bool m_transform; //变形标志 左键按下不松开标志，不松开true，松开false
    QPointF m_start;  //开始点
    QPointF m_end;    //结束点
    bool m_first;     //首次拉变

    bool m_hover;   //掠过标志
    qreal m_factor; //因数

signals:
    ///
    /// \brief detectChanged 信号：工具状态发生改变
    ///
    void detectChanged(QGraphicsItem *);
};

#endif // TOOLRECT_H
