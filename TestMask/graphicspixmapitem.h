#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsItem>

class GraphicsPixmapItem : public QGraphicsItem
{
public:
    enum
    {
        Type = QGraphicsItem::UserType + 7
    };

    GraphicsPixmapItem(QGraphicsItem *parent = nullptr);

    int type() const Q_DECL_OVERRIDE { return Type; }
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(
        QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    // void setTemplateRect(const QRectF &rect) { m_templateRect = rect; }
    void setTemplateDisplay(bool show, int alpha);
    void setTemplatePath(const QPainterPath &path) { m_path = path; }

    void setPixmap(const QImage &img);
    QImage image() { return m_image; } //隐式共享，无需引用

private:
    // QRectF m_templateRect;
    QPainterPath m_path;
    bool m_templateShow = true;
    int m_templateAlpha = 20;

    QImage m_image;
};

#endif // GRAPHICSPIXMAPITEM_H
