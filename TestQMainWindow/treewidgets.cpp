#include "treewidgets.h"

#include "ui_treewidgets.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

TreeWidgets::TreeWidgets(QWidget *parent) : QMainWindow(parent), ui(new Ui::TreeWidgets)
{
    ui->setupUi(this);
    resize(1400, 900);
    ui->treeFiles->clear();
    labFileName = new QLabel("");
    ui->statusbar->addWidget(labFileName);
    this->setCentralWidget(ui->scrollArea);
    initTree();
}

TreeWidgets::~TreeWidgets()
{
    delete ui;
}

void TreeWidgets::initTree()
{
    QString dataStr = ""; // Item的Data存储的string
    ui->treeFiles->clear();
    QIcon icon;
    icon.addFile(":/Image/15.ico");

    QTreeWidgetItem *item = new QTreeWidgetItem(TreeWidgets::itTopItem);
    item->setIcon(TreeWidgets::colItem, icon);                 //第一列的图标
    item->setText(TreeWidgets::colItem, "图片文件");           //第一列的文字
    item->setText(TreeWidgets::colItemType, "type=itTopItem"); //第二列
    // ItemIsSelectable是可选，按上蓝色，不按无色
    item->setFlags(Qt::ItemIsSelectable /*| Qt::ItemIsUserCheckable*/ | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    //    item->setCheckState(colItem, Qt::Checked);

    item->setData(TreeWidgets::colItem, Qt::UserRole, QVariant(dataStr));
    ui->treeFiles->addTopLevelItem(item); //添加顶层节点
}

void TreeWidgets::addFolderItem(QTreeWidgetItem *parItem, QString dirName)
{
    //添加一个组节点
    QIcon icon(":/Image/open.bmp");
    QString NodeText = getFinalFolderName(dirName);

    QTreeWidgetItem *item;
    item = new QTreeWidgetItem(TreeWidgets::itGroupItem); //节点类型
    item->setIcon(colItem, icon);
    item->setText(colItem, NodeText);
    item->setText(colItemType, "type=itGroupItem");
    item->setFlags(Qt::ItemIsSelectable /* | Qt::ItemIsUserCheckable*/ | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    //    item->setCheckState(colItem, Qt::Checked);
    item->setData(colItem, Qt::UserRole, QVariant(dirName));
    parItem->addChild(item); //在父节点下添加子节点
}

QString TreeWidgets::getFinalFolderName(const QString &fullPathName)
{
    //从一个完整目录路径，获取文件夹名称
    int cnt     = fullPathName.length();
    int i       = fullPathName.lastIndexOf("/");
    QString str = fullPathName.right(cnt - i - 1);
    qDebug() << "str:" << str;
    return str;
}

void TreeWidgets::addImageItem(QTreeWidgetItem *parItem, QString aFilename)
{
    //添加一个图片文件节点
    QIcon icon(":/Image/31.ico");
    QString NodeText = getFinalFolderName(aFilename); //获取最后的文件名称
    QTreeWidgetItem *item;
    item = new QTreeWidgetItem(TreeWidgets::itImageItem); //节点类型
    item->setIcon(colItem, icon);
    item->setText(colItem, NodeText);
    item->setText(colItemType, "type=itImageItem");
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
    item->setCheckState(colItem, Qt::Unchecked);
    item->setData(colItem, Qt::UserRole, QVariant(aFilename)); //完整文件名称
    parItem->addChild(item);                                   //父节点下添加子节点
}

void TreeWidgets::displayImage(QTreeWidgetItem *item)
{
    //显示图片
    QString filename = item->data(colItem, Qt::UserRole).toString(); //文件路径
    labFileName->setText(filename);
    curPixmap.load(filename);
    on_actionZoomFitH_triggered();
}

void TreeWidgets::changeItemCaption(QTreeWidgetItem *item)
{
    //改变节点的标题文字
    QString str = "*" + item->text(colItem); //节点标题前加"*"
    item->setText(colItem, str);
    if (item->childCount() > 0)
    {
        for (int i = 0; i < item->childCount(); i++) //遍历子节点
            changeItemCaption(item->child(i));       //调用自己
    }
}

void TreeWidgets::on_actionAddFolder_triggered()
{
    //添加组节点
    QTreeWidgetItem *parItem = ui->treeFiles->currentItem(); //选择的当前节点
    if (parItem == nullptr)
    {
        QMessageBox::critical(this, "注意", "请选择要创建的节点路径！", QMessageBox::Ok);
        return;
    }
    QString dir = QFileDialog::getExistingDirectory(); //选择目录
    if (!dir.isEmpty())
    {
        //添加节点
        addFolderItem(parItem, dir);
    }
}

void TreeWidgets::on_actionAddFiles_triggered()
{
    QTreeWidgetItem *parItem, *item;
    item = ui->treeFiles->currentItem();
    if (item == nullptr)
    {
        QMessageBox::critical(this, "注意", "请选择要创建的节点路径！", QMessageBox::Ok);
        return;
    }
    if (item->type() == itImageItem) //当前节点是图片节点
    { parItem = item->parent(); }
    else
    {
        parItem = item;
    }

    //添加图片文件节点
    QStringList files = QFileDialog::getOpenFileNames(this, "选择一个或多个文件", "", "Images(*.jpg;*.bmp;*.png)");
    if (files.isEmpty()) return;

    for (int i = 0; i < files.size(); ++i)
    {
        QString aFilename = files.at(i); //得到一个文件名
        addImageItem(parItem, aFilename);
    }
}

void TreeWidgets::on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current == nullptr) return;
    int var = current->type(); //当前节点类型
    switch (var)
    {
    case itTopItem: //顶层节点
        ui->actionAddFolder->setEnabled(true);
        ui->actionAddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(false); //顶层节点不能删除
        break;
    case itGroupItem: //组节点
        ui->actionAddFolder->setEnabled(true);
        ui->actionAddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(true);
        break;
    case itImageItem: //组节点
        ui->actionAddFolder->setEnabled(false);
        ui->actionAddFiles->setEnabled(true);
        ui->actionDeleteItem->setEnabled(true);
        displayImage(current); //显示图片
        break;
    }
}

void TreeWidgets::on_actionDeleteItem_triggered()
{
    //删除节点
    QTreeWidgetItem *item = ui->treeFiles->currentItem();
    if (item == nullptr) return;
    QTreeWidgetItem *parItem = item->parent();
    if (parItem == nullptr) return;
    parItem->removeChild(item); //移除一个子节点，并不会删除
    delete parItem;
}

void TreeWidgets::on_actionScanItems_triggered()
{
    //遍历节点
    for (int i = 0; i < ui->treeFiles->topLevelItemCount(); i++)
    {
        QTreeWidgetItem *item = ui->treeFiles->topLevelItem(i); //顶层节点
        changeItemCaption(item);                                //更改节点标题
    }
}

void TreeWidgets::on_actionZoomFitH_triggered()
{
    int h       = ui->scrollArea->height();
    int realh   = curPixmap.height();
    pixRatio    = float(h) / realh;                 //当前显示比例
    QPixmap pix = curPixmap.scaledToHeight(h - 30); //图片缩放到指定高度
    ui->label->setPixmap(pix);
}

void TreeWidgets::on_actionZoomFitW_triggered()
{
    int w       = ui->scrollArea->width();
    int realw   = curPixmap.width();
    pixRatio    = float(w) / realw;                //当前显示比例
    QPixmap pix = curPixmap.scaledToWidth(w - 30); //图片缩放到指定高度
    ui->label->setPixmap(pix);
}

void TreeWidgets::on_actionZoomIn_triggered()
{
    //放大显示
    pixRatio *= 1.2;
    int w       = pixRatio * curPixmap.width();
    int h       = pixRatio * curPixmap.height();
    QPixmap pix = curPixmap.scaled(w, h);
    ui->label->setPixmap(pix);
}

void TreeWidgets::on_actionZoomOut_triggered()
{
    pixRatio *= 0.8;
    int w       = pixRatio * curPixmap.width();
    int h       = pixRatio * curPixmap.height();
    QPixmap pix = curPixmap.scaled(w, h);
    ui->label->setPixmap(pix);
}

void TreeWidgets::on_actionZoomRealSize_triggered()
{
    pixRatio = 1;
    ui->label->setPixmap(curPixmap);
}

void TreeWidgets::on_actionDockVisible_toggled(bool arg1)
{
    //停靠区是否可见
    ui->dockWidget->setVisible(arg1);
}

void TreeWidgets::on_actionDockFloat_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}

void TreeWidgets::on_dockWidget_visibilityChanged(bool visible)
{
    //停靠区关闭信号
    ui->actionDockVisible->setChecked(visible);
}

void TreeWidgets::on_dockWidget_topLevelChanged(bool topLevel)
{
    //停靠区浮动变化
    ui->actionDockFloat->setChecked(topLevel);
}
