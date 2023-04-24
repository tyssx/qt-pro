#ifndef BUTTONDELEGATE_H
#define BUTTONDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class ButtonDelegate : public QStyledItemDelegate
{
public:
    explicit ButtonDelegate();
    ~ButtonDelegate();

    void paint(QPainter *painter,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const override;

    bool editorEvent(QEvent *event,
        QAbstractItemModel *model,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) override;
};

#endif // BUTTONDELEGATE_H
