#ifndef TABLEFROM_H
#define TABLEFROM_H

#include "qwintspindelegate.h"

#include <QMainWindow>

class QStandardItemModel;

namespace Ui
{
class TableForm;
}

class TableForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableForm(QWidget *parent = nullptr);
    ~TableForm();

private:
    void initTable();

private slots:
    void slot_actionNew();

    void slot_actionSave();

private:
    Ui::TableForm *ui;
    QStandardItemModel *m_itemModel;
    QWIntSpinDelegate intSpinDelegate;
    int m_row;
    int m_column;
};

#endif // TABLEFROM_H
