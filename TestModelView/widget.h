#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QFileSystemModel;
class QStringListModel;
class TableForm;

namespace Ui
{
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_pushButtonAppendListItem_clicked();

    void on_pushButtonInsertListItem_clicked();

    void on_pushButtonDeleteListCurItem_clicked();

    void on_pushButtonClearAllItem_clicked();

    void on_listView_clicked(const QModelIndex &index);

private:
    Ui::Widget *ui;
    QFileSystemModel *m_fileModel;
    QStringListModel *m_stringModel;
    TableForm *m_tableForm;
};

#endif // WIDGET_H
