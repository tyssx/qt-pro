#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
class QLabel;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

    void addChart(QWidget *);

protected:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void slot_zoomall();

private:
    bool m_drag;       //右键拖动视野
    QLabel *m_labelXY; //当前xy坐标
    QPoint m_start;    //右键移动时的起始点
};

#endif // GRAPHICSVIEW_H
