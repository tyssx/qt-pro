#include "widget.h"

#include "customtabstyle.h"
#include "tableform.h"
#include "ui_widget.h"

#include <QFileSystemModel>
#include <QStringListModel>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    QTabBar *tab = ui->tabWidget->tabBar();
    tab->setStyle(new CustomTabStyle);

    m_fileModel = new QFileSystemModel(this);
    m_fileModel->setRootPath(QDir::rootPath());
    ui->treeView->setModel(m_fileModel); //设置数据模型

    QStringList stringList;
    stringList << "图像1"
               << "图像2"
               << "图像3"
               << "图像4";
    m_stringModel = new QStringListModel(this);
    m_stringModel->setStringList(stringList);
    ui->listView->setModel(m_stringModel);
    ui->listView->setEditTriggers(
        QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);

    QVBoxLayout *layout = new QVBoxLayout;
    m_tableForm         = new TableForm;
    layout->addWidget(m_tableForm);
    ui->tabTable->setLayout(layout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_treeView_clicked(const QModelIndex &index)
{
    ui->checkBoxIsDir->setChecked(m_fileModel->isDir(index));
    ui->labelPath->setText(m_fileModel->filePath(index));
    ui->labelName->setText(m_fileModel->fileName(index));
    ui->labelType->setText(m_fileModel->type(index));
    int size = m_fileModel->size(index) / 1024;
    if (size < 1024)
        ui->labelSize->setText(QString("%1 KB").arg(size));
    else
    {
        ui->labelSize->setText(QString::asprintf("%.lf MB", size / 1024.0));
    }
}

void Widget::on_pushButtonAppendListItem_clicked()
{
    //尾部插入行
    m_stringModel->insertRow(m_stringModel->rowCount());
    int curRow        = m_stringModel->rowCount() - 1;
    QModelIndex index = m_stringModel->index(curRow, 0);
    m_stringModel->setData(
        index, QVariant(QString::asprintf("图像%d", curRow + 1)), Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void Widget::on_pushButtonInsertListItem_clicked()
{
    QModelIndex curIndex = ui->listView->currentIndex();
    int curRow           = curIndex.row();
    m_stringModel->insertRow(curRow + 1);
    QModelIndex newIndex = m_stringModel->index(curRow + 1, 0);
    m_stringModel->setData(newIndex, tr("新图像"), Qt::DisplayRole);
    ui->listView->setCurrentIndex(newIndex);
}

void Widget::on_pushButtonDeleteListCurItem_clicked()
{
    QModelIndex curIndex = ui->listView->currentIndex();
    m_stringModel->removeRow(curIndex.row());
}

void Widget::on_pushButtonClearAllItem_clicked()
{
    m_stringModel->removeRows(0, m_stringModel->rowCount());
}

void Widget::on_listView_clicked(const QModelIndex &index)
{
    ui->labelListContent->setText(index.data(Qt::DisplayRole).toString());
}
