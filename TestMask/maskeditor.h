#ifndef MASKEDITOR_H
#define MASKEDITOR_H

#include <QMainWindow>

class MaskScene;
class GraphicsPixmapItem;
class QActionGroup;
class MaskPathItem;
class QLabel;
class QUndoStack;
class EsviDataItem;
class DuGroup;

namespace Ui
{
class MaskEditor;
}

class MaskEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit MaskEditor(QWidget *parent = nullptr);
    ~MaskEditor();

private slots:
    void on_actionClear_triggered();
    void on_actionReverse_triggered();
    void slot_maskMerge();
    void slot_indicatorChanged();

private:
    Ui::MaskEditor *ui;

    QLabel *m_labelInfo;
    bool m_first;

    QActionGroup *m_actionGroupTool;    //形状工具组
    QActionGroup *m_actionGroupOperate; //运算操作组

    MaskScene *m_scene;
    GraphicsPixmapItem *m_pixmapItem;
    EsviDataItem *m_dataItem; //结果
    QPolygonF m_polygon;      //边界

    MaskPathItem *m_document; // mask轮廓
    QUndoStack *m_stack;      //命令栈
};

#endif // MASKEDITOR_H
