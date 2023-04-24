#ifndef MASKINDICATOR_H
#define MASKINDICATOR_H

#include <QGraphicsObject>

class MaskIndicator : public QGraphicsObject
{
public:
    enum Tool
    {
        None,
        Rectangle,
        Ellipse,
        Polygon
    };
    explicit MaskIndicator(QGraphicsItem *parent = nullptr);
    virtual ~MaskIndicator();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    virtual void paint(
        QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    Tool m_tool = None;
    QPainterPath m_contour;
    QPolygonF m_polygon;
    qreal m_factor  = 1; //因数
    bool m_isFinish = false;
};

#endif // MASKINDICATOR_H
