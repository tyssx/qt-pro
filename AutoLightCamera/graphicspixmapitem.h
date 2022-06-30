#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsPixmapItem>
//就是为了显示一个边框，其实设置为selected也会显示边框，但会带来其它的一些问题
class GraphicsPixmapItem : public QGraphicsPixmapItem
{
public:
    ///
    /// \brief GraphicsPixmapItem 构造图像Item,默认在图像边界显示一个黑框
    /// \param parent 指定父Item
    ///
    GraphicsPixmapItem(QGraphicsItem *parent = nullptr);

    ///
    /// \brief paint 绘制项目的内容
    /// \param painter 绘图板
    /// \param option 绘图选项
    /// \param widget 正在绘制的小部件
    ///
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setTemplateRect(const QRectF &rect) { m_templateRect = rect; }

private:
    QRectF m_templateRect;
};

#endif // GRAPHICSPIXMAPITEM_H
