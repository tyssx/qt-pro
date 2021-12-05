#include "tablewidgets.h"

#include "ui_tablewidgets.h"

#include <QDebug>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

TableWidgets::TableWidgets(QWidget *parent) : QMainWindow(parent), ui(new Ui::TableWidgets)
{
    ui->setupUi(this);
    // TableWidgets要有内容首先要设置行列数，已经在ui文件中设置了行列
    resize(1200, 800);
    setCentralWidget(ui->splitterMain);
    //设置分裂器比例
    ui->splitterMain->setStretchFactor(0, 3);
    ui->splitterMain->setStretchFactor(1, 7);

    labCellIndex = new QLabel("当前单元格坐标：", this);
    labCellIndex->setMinimumWidth(250);
    labCellType = new QLabel("当前单元格类型：", this);
    labCellType->setMinimumWidth(200);
    labStudID = new QLabel("学生ID：", this);
    labStudID->setMinimumWidth(200);
    ui->statusbar->addWidget(labCellIndex);
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStudID);
    //间隔底色交替
    ui->tableInfo->setAlternatingRowColors(true);
    ui->pushButtonAutoSetH->setCheckable(true);
    ui->pushButtonAutoSetW->setCheckable(true);
}

TableWidgets::~TableWidgets()
{
    delete ui;
}

void TableWidgets::createItemsARow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isPM, int score)
{
    QTableWidgetItem *item;
    QString str;
    uint StuID = 201605000;
    //姓名
    item = new QTableWidgetItem(name, ctName);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    StuID += rowNo;
    item->setData(Qt::UserRole, QVariant(StuID));
    ui->tableInfo->setItem(rowNo, colName, item);
    //性别
    QIcon icon;
    if (sex == "男")
        icon.addFile(":/Image/boy.ico");
    else
    {
        icon.addFile(":/Image/girl.ico");
    }
    item = new QTableWidgetItem(sex, ctSex);
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, colSex, item);

    //出生日期
    str  = birth.toString("yyyy-MM-dd");
    item = new QTableWidgetItem(str, ctBirth);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, colBirth, item);
    //民族
    item = new QTableWidgetItem(nation, ctNation);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, colNatiom, item);
    //是否党员
    item = new QTableWidgetItem("党员", ctPartyM);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
    {
        item->setCheckState(Qt::Unchecked);
    }
    item->setBackground(Qt::yellow);
    ui->tableInfo->setItem(rowNo, colPartyM, item);
    //分数
    str  = QString::number(score);
    item = new QTableWidgetItem(str, ctScore);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableInfo->setItem(rowNo, colScore, item);
}

void TableWidgets::on_pushButtonSetHeader_clicked()
{
    //设置表头
    QTableWidgetItem *headerItem;
    QStringList headerText;
    headerText << "姓名"
               << "性别"
               << "出生日期"
               << "民族"
               << "分数"
               << "是否党员";
    //    ui->tableInfo->setHorizontalHeaderLabels(headerText);
    ui->tableInfo->setColumnCount(headerText.count());
    for (int i = 0; i < ui->tableInfo->columnCount(); i++)
    {
        headerItem = new QTableWidgetItem(headerText.at(i));
        QFont font = headerItem->font();
        font.setBold(true); //设置粗体
        font.setPointSize(12);
        headerItem->setTextColor(Qt::red);
        headerItem->setFont(font);
        ui->tableInfo->setHorizontalHeaderItem(i, headerItem);
    }
}

void TableWidgets::on_pushButtonSetRowCount_clicked()
{
    int rowCount = ui->spinBoxRowCount->value();
    ui->tableInfo->setRowCount(rowCount);
}

void TableWidgets::on_pushButtonSetColumnCount_clicked()
{
    int columnCount = ui->spinBoxColumnCount->value();
    ui->tableInfo->setColumnCount(columnCount);
}

void TableWidgets::on_pushButtonInitData_clicked()
{
    //初始化表格数据
    QString strName, strSex;
    bool isParty = false;
    QDate birth;
    birth.setDate(1980, 4, 8);      //初始化日期
    ui->tableInfo->clearContents(); //清除工作区，不清除表头
    int rows = ui->tableInfo->rowCount();
    qDebug() << "rows" << rows;
    for (int i = 0; i < rows; i++)
    {
        strName = QString::asprintf("学生%d", i);
        if ((i % 2) == 0)
            strSex = "男";
        else
        {
            strSex = "女";
        }
        createItemsARow(i, strName, strSex, birth, "汉族", isParty, 70);
        birth   = birth.addDays(20);
        isParty = !isParty;
    }
}

void TableWidgets::on_pushButtonInsertRow_clicked()
{
    //插入一行
    int curRow = ui->tableInfo->currentRow();
    ui->tableInfo->insertRow(curRow); //插入一行，不会为自动创建item
    createItemsARow(curRow, "新学生", "男", QDate::fromString("1990-01-01", "yyyy-MM-dd"), "苗族", true, 90);
}

void TableWidgets::on_pushButtonAppendRow_clicked()
{
    int curRow = ui->tableInfo->rowCount();
    ui->tableInfo->insertRow(curRow); //插入一行，不会自动创建item
    createItemsARow(curRow, "新同学", "女", QDate::fromString("1993-05-21", "yyyy-MM-dd"), "东乡族", true, 90);
}

void TableWidgets::on_pushButtonDeleteRow_clicked()
{
    int curRow = ui->tableInfo->currentRow();
    ui->tableInfo->removeRow(curRow); //删除行
}

void TableWidgets::on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow);
    Q_UNUSED(previousColumn);

    QTableWidgetItem *item = ui->tableInfo->item(currentRow, currentColumn);
    if (item == nullptr) return;
    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d 行 %d 列", currentRow, currentColumn));
    int cellType = item->type();
    labCellType->setText(QString::asprintf("当前单元格类型：%d", cellType));
    item   = ui->tableInfo->item(currentRow, colName); //第一列item
    int ID = item->data(Qt::UserRole).toInt();
    labStudID->setText(QString::asprintf("学生ID：%d", ID));
}

void TableWidgets::on_pushButtonAutoSetH_clicked()
{
    ui->tableInfo->resizeRowsToContents(); //自动调整行高
}

void TableWidgets::on_pushButtonAutoSetW_clicked()
{
    ui->tableInfo->resizeColumnsToContents(); //自动调整列宽
}

void TableWidgets::on_pushButtonGetContentToText_clicked()
{
    QString str;
    QTableWidgetItem *cellItem;
    ui->plainTextEdit->clear();
    for (int i = 0; i < ui->tableInfo->rowCount(); i++)
    {
        str = str.sprintf("第 %d 行 ", i + 1);
        for (int j = 0; j < ui->tableInfo->columnCount(); j++)
        {
            cellItem = ui->tableInfo->item(i, j);
            str      = str + cellItem->text() + "  "; //字符串连接
        }
        cellItem = ui->tableInfo->item(i, colPartyM);
        if (cellItem->checkState() == Qt::Checked)
            str = str + "党员";
        else
            str = str + "群众";
        ui->plainTextEdit->appendPlainText(str);
    }
}

void TableWidgets::on_checkBoxTableCanEdit_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    //是否可编辑
    if (ui->checkBoxTableCanEdit->checkState())
    {
        //双击或获取焦点后单击，进入编辑状态
        ui->tableInfo->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    }
    else
    {
        ui->tableInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }
}

void TableWidgets::on_checkBoxShowRowHeader_clicked(bool checked)
{
    //是否显示行表头
    ui->tableInfo->horizontalHeader()->setVisible(checked);
}

void TableWidgets::on_checkBoxShowColumnHeader_clicked(bool checked)
{
    //是否显示列表头
    ui->tableInfo->verticalHeader()->setVisible(checked);
}

void TableWidgets::on_checkBoxAlterRowColor_clicked(bool checked)
{
    //间隔行底色
    ui->tableInfo->setAlternatingRowColors(checked);
}

void TableWidgets::on_radioButtonRowChoice_clicked()
{
    //行选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void TableWidgets::on_radioButtonTableChoice_clicked()
{
    //单元格选择
    ui->tableInfo->setSelectionBehavior(QAbstractItemView::SelectItems);
}
