#include "tableform.h"

#include "setrcdialog.h"
#include "ui_tableform.h"

#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QToolBar>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

TableForm::TableForm(QWidget *parent) : QMainWindow(parent), ui(new Ui::TableForm)
{
    ui->setupUi(this);

    QToolBar *toolBar = new QToolBar(this);
    this->addToolBar(Qt::TopToolBarArea, toolBar);
    toolBar->addAction(ui->actionNew);
    toolBar->addAction(ui->actionSave);
    toolBar->addAction(ui->actionAddRow);
    toolBar->addAction(ui->actionAddColumn);
    toolBar->addAction(ui->actionInsertRow);
    toolBar->addAction(ui->actionInsertColumn);

    m_itemModel = new QStandardItemModel(6, 4, this);
    ui->tableView->setModel(m_itemModel);
    ui->tableView->setItemDelegateForColumn(4, &intSpinDelegate);

    initTable();

    connect(ui->actionNew, &QAction::triggered, this, &TableForm::slot_actionNew);
    connect(ui->actionSave, &QAction::triggered, this, &TableForm::slot_actionSave);
}

TableForm::~TableForm()
{
    delete ui;
}

void TableForm::initTable()
{
    QStringList strList;
    strList << "物料编号"
            << "物料类别"
            << "物料名称"
            << "单片价格"
            << "合格率";

    //设置表头
    m_itemModel->setHorizontalHeaderLabels(strList);
    QStandardItem *item;
    for (int i = 0; i < 6; i++)
    {
        item = new QStandardItem(QString::asprintf("00%d", i));
        m_itemModel->setItem(i, 0, item);
    }
    for (int i = 0; i < 6; i++)
    {
        QString str;
        if (i % 2 == 0)
            str = QString("常规");
        else
            str = QString("稀有");
        item = new QStandardItem(str);
        m_itemModel->setItem(i, 1, item);
    }
    for (int i = 0; i < 6; i++)
    {
        QStringList name;
        name << "贴片电阻"
             << "贴片陶瓷电容"
             << "贴片电感"
             << "贴片晶振"
             << "发光二极管"
             << "拨码开关";
        item = new QStandardItem(name.at(i));
        m_itemModel->setItem(i, 2, item);
    }
    for (int i = 0; i < 6; i++)
    {
        item = new QStandardItem(QString::asprintf("%lf", i * i + 0.256));
        m_itemModel->setItem(i, 3, item);
    }
    for (int i = 0; i < 6; i++)
    {
        item = new QStandardItem(QString::asprintf("%d", i + 90));
        m_itemModel->setItem(i, 4, item);
    }
}

void TableForm::slot_actionNew()
{
    SetRCDialog setRcDlg;
    connect(&setRcDlg, &SetRCDialog::OkSignal, this, [=](int row, int column, QStringList strList) {
        m_itemModel->setRowCount(row);
        m_itemModel->setColumnCount(column);
        if (strList.size() != 0)
            m_itemModel->setHorizontalHeaderLabels(strList);
        else
            ui->tableView->horizontalHeader()->setHidden(true);
    });
    setRcDlg.exec();
    //    int result = setRcDlg.exec();
    //    if (result == QDialog::Accepted)
    //    {
    //        setRcDlg.getRowCol(m_row, m_column);
    //        qDebug() << "m_row" << m_row << "m_column" << m_column;
    //        if (m_itemModel)
    //        {
    //            m_itemModel->setRowCount(m_row);
    //            m_itemModel->setColumnCount(m_column);
    //        }
    //    }
}

void TableForm::slot_actionSave()
{
    QString fileName = QFileDialog::getSaveFileName(this, "保存文件", "./new.txt", "Text (*.txt)");
}
