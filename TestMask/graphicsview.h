#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
class QLabel;
class QToolButton;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(QWidget *parent = nullptr);

    void addChart(QWidget *);
    void setZoomButtonVisible(bool visible);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void drawForeground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

protected slots:
    void slot_zoomin();
    void slot_zoomout();
    void slot_zoom1();
    void slot_zoomall();

private:
    bool m_drag;       //右键拖动视野
    QLabel *m_labelXY; //当前xy坐标，RGB值
    QPoint m_start;    //右键移动时的起始点
    QToolButton *m_zoomButton;

signals:

    void mouseClicked(QPoint point); //现在仅在imageform有使用，要想办法去掉
};

#endif // GRAPHICSVIEW_H
