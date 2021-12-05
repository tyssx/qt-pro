#include "listwidgets.h"

#include "ui_listwidgets.h"

#include <QDockWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMenuBar>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

ListWidgets::ListWidgets(QWidget *parent) : QMainWindow(parent), ui(new Ui::ListWidgets)
{
    ui->setupUi(this);
    setWindowTitle("item Wigets界面");

    createMenuToolBar(0);
    // toolBox和tabWidget绑定
    connect(ui->toolBox, &QToolBox::currentChanged, this, &ListWidgets::slot_toolBoxChanged);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &ListWidgets::slot_tabWidgetChanged);
}

ListWidgets::~ListWidgets()
{
    delete ui;
}

void ListWidgets::createSelectionPopMenu()
{
    //创建下拉菜单
    QMenu *menuSelection = new QMenu(this);
    menuSelection->addAction(ui->actionSelAll);
    menuSelection->addAction(ui->actionSelNone);
    menuSelection->addAction(ui->actionSelInvs);
    // QListWidget选择项
    ui->toolButtonSelItem->setPopupMode(QToolButton::MenuButtonPopup);
    ui->toolButtonSelItem->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolButtonSelItem->setDefaultAction(ui->actionSelPopMenu); //关联Action
    ui->toolButtonSelItem->setMenu(menuSelection);                 // button设置菜单
    //工具栏下拉菜单
    QToolButton *toolButton = new QToolButton(this);
    toolButton->setPopupMode(QToolButton::InstantPopup);
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon); //按钮样式
    toolButton->setDefaultAction(ui->actionSelPopMenu);
    toolButton->setMenu(menuSelection); // button设置菜单
    ui->toolBar->addWidget(toolButton); // 工具栏添加按钮
    //添加分隔符
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actionQuit);
    connect(ui->actionSelPopMenu, &QAction::triggered, ui->actionSelAll, &QAction::trigger); //设置默认的触发项
}

void ListWidgets::createMenuToolBar(int index)
{
    switch (index)
    {
    case 0:
    {
        //        QActionGroup *listGroup = new QActionGroup(this);
        //        listGroup->addAction(ui->actionListInit);
        //        listGroup->addAction(ui->actionListClear);
        //        listGroup->addAction(ui->actionListInsert);
        //        listGroup->addAction(ui->actionListAppend);
        //        listGroup->addAction(ui->actionListDelete);
        listList.clear();
        listList.append(ui->actionListInit);
        listList.append(ui->actionListClear);
        listList.append(ui->actionListInsert);
        listList.append(ui->actionListAppend);
        listList.append(ui->actionListDelete);
        ui->toolBar->clear();
        ui->toolBar->addActions(listList);

        ui->toolButtonListInit->setDefaultAction(ui->actionListInit);
        ui->toolButtonListClear->setDefaultAction(ui->actionListClear);
        ui->toolButtonListInsert->setDefaultAction(ui->actionListInsert);
        ui->toolButtonListAppend->setDefaultAction(ui->actionListAppend);
        ui->toolButtonListDelete->setDefaultAction(ui->actionListDelete);

        ui->toolButtonSelAll->setDefaultAction(ui->actionSelAll);
        ui->toolButtonSelNone->setDefaultAction(ui->actionSelNone);
        ui->toolButtonSelInvs->setDefaultAction(ui->actionSelInvs);
        createSelectionPopMenu();
    }
    break;
    case 1:
    {
        QMenuBar *treeMenuBar = menuBar();
        treeMenuBar->clear();
        QMenu *treeMenu = treeMenuBar->addMenu("目录树");
        QMenu *viewMenu = treeMenuBar->addMenu("视图");

        treeMenu->addAction(ui->actionAddFolder);
        treeMenu->addAction(ui->actionAddFiles);
        viewMenu->addAction(ui->actionZoomIn);
        viewMenu->addAction(ui->actionZoomOut);
        listList.clear();
        listList.append(ui->actionAddFolder);
        listList.append(ui->actionAddFiles);
        listList.append(ui->actionZoomIn);
        listList.append(ui->actionZoomOut);
        ui->toolBar->clear();
        ui->toolBar->addActions(listList);
    }
    break;
    case 2:
    {
    }
    break;
    }
}

void ListWidgets::on_actionListInit_triggered()
{
    QListWidgetItem *aItem;                          // 每一行是一个item
    bool canEdit = ui->checkBoxCanEdit->isChecked(); //可编辑被选择
    ui->listWidget->clear();
    for (int i = 0; i < 10; i++)
    {
        QString str = QString::asprintf("item %d", i);
        aItem       = new QListWidgetItem;
        aItem->setText(str);
        aItem->setCheckState(Qt::Checked); //设置选中状态
        if (canEdit)
        {
            // ItemIsSelectable可被选择 ItemIsEditable可编辑 ItemIsUserCheckable可复选 Enabled使能
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        }
        else
        { //不可编辑
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        }
        ui->listWidget->addItem(aItem); //增加项
    }
}

void ListWidgets::on_actionListClear_triggered()
{
    ui->listWidget->clear(); //清空
}

void ListWidgets::on_actionListInsert_triggered()
{
    bool canEdit           = ui->checkBoxCanEdit->isChecked(); //可编辑被选择
    QListWidgetItem *aItem = new QListWidgetItem("New Insert Item");
    aItem->setCheckState(Qt::Checked);
    if (canEdit) { aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled); }
    else
    {
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
    ui->listWidget->insertItem(ui->listWidget->currentRow(), aItem);
}

void ListWidgets::on_actionListAppend_triggered()
{
    bool canEdit           = ui->checkBoxCanEdit->isChecked(); //可编辑被选择
    QListWidgetItem *aItem = new QListWidgetItem("New Append Item");
    aItem->setCheckState(Qt::Checked);
    if (canEdit) { aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled); }
    else
    {
        aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    }
    ui->listWidget->addItem(aItem);
}

void ListWidgets::on_actionListDelete_triggered()
{
    //删除当前项
    int row                = ui->listWidget->currentRow();
    QListWidgetItem *aItem = ui->listWidget->takeItem(row); // takeItem只是移除项，并不删除项对象，所以需要手动删除。
    delete aItem;                                           //手动删除对象
}

void ListWidgets::on_actionSelAll_triggered()
{
    //全选
    int cnt = ui->listWidget->count(); //所有项个数
    for (int i = 0; i < cnt; i++)
    {
        QListWidgetItem *aItem = ui->listWidget->item(i);
        aItem->setCheckState(Qt::Checked);
    }
}

void ListWidgets::on_actionSelNone_triggered()
{
    //取消全选
    int cnt = ui->listWidget->count(); //所有项个数
    for (int i = 0; i < cnt; i++)
    {
        QListWidgetItem *aItem = ui->listWidget->item(i);
        aItem->setCheckState(Qt::Unchecked);
    }
}

void ListWidgets::on_actionSelInvs_triggered()
{
    //反选
    int cnt = ui->listWidget->count(); //所有项个数
    for (int i = 0; i < cnt; i++)
    {
        QListWidgetItem *aItem = ui->listWidget->item(i);
        if (aItem->checkState() == Qt::Checked)
            aItem->setCheckState(Qt::Unchecked);
        else if (aItem->checkState() == Qt::Unchecked)
            aItem->setCheckState(Qt::Checked);
    }
}

void ListWidgets::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    //当前选中项发生变化
    QString str;
    if (current != nullptr)
    {
        if (previous == nullptr) { str = "当前项：" + current->text(); }
        else
        {
            str = "前一项：" + previous->text() + "；当前项：" + current->text();
        }
        ui->lineEditCutItemText->setText(str);
    }
}

void ListWidgets::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    //鼠标右键发射信号，生成菜单
    Q_UNUSED(pos);
    QMenu *menuList = new QMenu(this);
    menuList->addAction(ui->actionListInit);
    menuList->addAction(ui->actionListClear);
    menuList->addAction(ui->actionListInsert);
    menuList->addAction(ui->actionListAppend);
    menuList->addAction(ui->actionListClear);
    menuList->addSeparator();
    menuList->addAction(ui->actionSelAll);
    menuList->addAction(ui->actionSelNone);
    menuList->addAction(ui->actionSelInvs);
    menuList->exec(QCursor::pos()); //鼠标光标位置显示右键快捷菜单
    delete menuList;
}

void ListWidgets::slot_toolBoxChanged(int index)
{
    ui->tabWidget->setCurrentIndex(index);
    createMenuToolBar(index);
}

void ListWidgets::slot_tabWidgetChanged(int index)
{
    ui->toolBox->setCurrentIndex(index);
    createMenuToolBar(index);
}
