#ifndef ESVIDATAITEM_H
#define ESVIDATAITEM_H

#include "abstractdisplay.h"

#include <QGraphicsItem>
#include <QObject>

class EsviDataItem : public QObject, public QGraphicsItem, public AbstractDisplay
{
    Q_OBJECT
public:
    explicit EsviDataItem(QObject *parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setExtEdge(const std::vector<Stroke> &ctr) override;
    void setExtSegment(const std::vector<Stroke> &ctr) override;
signals:

public slots:

private:
    mutable QRectF m_boundingRect;
    QPainterPath m_extEdge; //原型边缘
    QColor m_color;
    Qt::PenStyle m_penStyle;
    std::vector<QPainterPath> m_extSegment; //分割区域边缘
};

#endif // ESVIDATAITEM_H
