#include "graphicsscene.h"

#include "extvariation3chitem.h"

#include <QDebug>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

using std::string;
using std::vector;

GraphicsScene::GraphicsScene(QObject *parent) : QGraphicsScene(parent)
{
}

GraphicsScene::~GraphicsScene()
{
}

void GraphicsScene::setCurTool(int tool)
{
    if (m_curItem)
    {
        // removeItem(m_curItem);
        delete m_curItem;
        m_curItem = nullptr;
        qDebug() << "删除了一个没完成的工具item";
    }

    //清除原有关联
    this->disconnect(SIGNAL(ctrlPoint(QPointF, /*DataUnit *,*/ bool)));

    //根据工具的类型确定交互方式及指示器的表现形式
    // m_curItem 此时一定为null
    m_curTool    = tool;
    m_mode       = Default;
    bool canEdit = true;
    switch (m_curTool)
    {
    case 0:
        m_mode  = Default;
        canEdit = true;
        break;
    case 1:
        m_curItem = new ExtVariation3ChItem;
        m_mode    = ItemSelf;
        m_capture = -1;
        break;
    }
    if (m_curItem)
    {
        addItem((QGraphicsItem *)m_curItem);
        m_curItem->setZValue(1);
        connect(this, &GraphicsScene::ctrlPoint, m_curItem, &ExtVariation3ChItem::buildFromCtrlpoint);
    }
}

void GraphicsScene::addOrDel()
{
    Q_ASSERT(m_curItem);

    //根据item建立过程中的状态，确定处理方式:
    ExtVariation3ChItem::State st = m_curItem->buildState();
    if (st == ExtVariation3ChItem::Success)
    { //正式确认加入新建对象
        emit result(m_curItem);
        m_curItem = nullptr;
        setCurTool(0);
    }
    else if (st == ExtVariation3ChItem::Failed)
    { //删除
        delete m_curItem;
        m_curItem = nullptr;
        emit result(m_curItem);
        // m_curItem = nullptr;
        setCurTool(0);
    }
    else
    {
        //其它情况不处理
    }
}

void GraphicsScene::del()
{
    delete m_curItem;
    m_curItem = nullptr;
    emit result(m_curItem);
    // m_curItem = nullptr;
    setCurTool(0);
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // item自处理
    if (m_mode == ItemSelf && mouseEvent->button() == Qt::LeftButton)
    {
        QPointF pt = mouseEvent->scenePos();
        emit ctrlPoint(pt, /*nullptr, */ false);
        m_capture = 0;

        addOrDel(); //根据返回情况，正式确认加入新建对象或删除

        return;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == ItemSelf && m_capture == 1)
    {
        QPointF pt = mouseEvent->scenePos();
        emit ctrlPoint(pt, /*nullptr, */ true);
        return;
    }

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (m_mode == ItemSelf && m_capture == 0 && mouseEvent->button() == Qt::LeftButton)
    {
        m_capture = 1;
        return;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
