#ifndef LISTWIDGETS_H
#define LISTWIDGETS_H

#include <QMainWindow>

class QListWidgetItem;

namespace Ui
{
class ListWidgets;
}

class ListWidgets : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListWidgets(QWidget *parent = nullptr);
    ~ListWidgets();

private slots:
    void on_actionListInit_triggered();

    void on_actionListClear_triggered();

    void on_actionListInsert_triggered();

    void on_actionListAppend_triggered();

    void on_actionListDelete_triggered();

    void on_actionSelAll_triggered();

    void on_actionSelNone_triggered();

    void on_actionSelInvs_triggered();

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void slot_toolBoxChanged(int index);

    void slot_tabWidgetChanged(int index);

private:
    void createSelectionPopMenu();
    void createMenuToolBar(int index);

private:
    Ui::ListWidgets *ui;

    QList<QAction *> listList;
};

#endif // LISTWIDGETS_H
