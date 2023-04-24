#include "qwintspindelegate.h"

#include <QSpinBox>

QWIntSpinDelegate::QWIntSpinDelegate(QObject *parent)
{
}

QWidget *QWIntSpinDelegate::createEditor(
    QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //创建代理编辑组件
    QSpinBox *editor = new QSpinBox(parent);
    editor->setFrame(false); //设置为无边框
    editor->setMinimum(0);
    editor->setMaximum(1000);
    return editor;
}

void QWIntSpinDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //从数据模型获取数据，显示到代理组件上
    int value         = index.model()->data(index, Qt::DisplayRole).toInt();
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    spinBox->setValue(value);
}

void QWIntSpinDelegate::setModelData(
    QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //代理数据保存到数据模型中
    QSpinBox *spinBox = qobject_cast<QSpinBox *>(editor);
    spinBox->interpretText(); //解读内容
    int value = spinBox->value();
    model->setData(index, value, Qt::EditRole);
}

void QWIntSpinDelegate::updateEditorGeometry(
    QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //设置代理组件大小
    editor->setGeometry(option.rect);
}
