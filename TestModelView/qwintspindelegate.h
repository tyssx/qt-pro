#ifndef QWINTSPINDELEGATE_H
#define QWINTSPINDELEGATE_H

#include <QStyledItemDelegate>

class QWIntSpinDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QWIntSpinDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const override; //用于编辑模型数据的widget组件

    void setEditorData(
        QWidget *editor, const QModelIndex &index) const override; //从数据模型获取值设置为显示值

    void setModelData(QWidget *editor,
        QAbstractItemModel *model,
        const QModelIndex &index) const override; //从代理编辑器上获取值设置到模型数据

    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const override; //设置代理编辑器的大小
};

#endif // QWINTSPINDELEGATE_H
