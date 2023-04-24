#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>

class QGraphicsPathItem;
class EsviHotItem;
class ExtVariation3ChItem;
class DataUnit;

class GraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    //交互方式
    // Default:系统默认方式
    // ItemSelf：Item本身维护，多点创建基本几何要素
    // Drag：点-拖动-释放，测量距离/角度等，拾取两个热点
    // MultiPick：单击-单击...-右键，拾取多个热点
    //交互时呈现的形状：线/圆/圆弧，待定义
    enum Mode
    {
        Default,
        ItemSelf,
        Drag,
        MultiPick
    };

    explicit GraphicsScene(QObject *parent = nullptr);
    ~GraphicsScene() override;
    void setCurTool(int tool);
    void addOrDel();
    void del();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

private:
    QGraphicsPathItem *m_indicator;     //指示器
    EsviHotItem *m_hotItem;             //热点
    std::vector<DataUnit *> m_vect;     //热点列表，与指示器对应
    int m_curTool                  = 0; //当前item
    ExtVariation3ChItem *m_curItem = nullptr;
    Mode m_mode                    = Default;
    int m_capture                  = -1; //多点方式时状态位
                                         //跟踪mouse状态
                                         //-1：未进入，
                                         // 0：锁定状态，屏蔽热点捕获
                                         // 1..n：第一击至n

signals:
    void result(ExtVariation3ChItem *);                         //交互的结果
    void ctrlPoint(QPointF pt, /* DataUnit *, */ bool isTrack); // item的建立控制点
};

#endif // GRAPHICSSCENE_H
