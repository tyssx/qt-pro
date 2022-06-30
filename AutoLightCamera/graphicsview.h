#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

private:
    bool m_drag;    //右键拖动视野
    QPoint m_start; //右键移动时的起始点
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event); //鼠标滚动放大缩小图像

signals:
    void mouseClicked(QPoint point); //现在仅在imageform有使用，要想办法去掉
};

#endif // GRAPHICSVIEW_H
