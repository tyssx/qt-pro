#ifndef TABLEWIDGETS_H
#define TABLEWIDGETS_H

#include <QDate>
#include <QLabel>
#include <QMainWindow>

namespace Ui
{
class TableWidgets;
}

class TableWidgets : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWidgets(QWidget *parent = nullptr);
    ~TableWidgets();

private slots:
    void on_pushButtonSetHeader_clicked();

    void on_pushButtonSetRowCount_clicked();

    void on_pushButtonSetColumnCount_clicked();

    void on_pushButtonInitData_clicked();

    void on_pushButtonInsertRow_clicked();

    void on_pushButtonAppendRow_clicked();

    void on_pushButtonDeleteRow_clicked();

    void on_pushButtonAutoSetH_clicked();

    void on_pushButtonAutoSetW_clicked();

    void on_pushButtonGetContentToText_clicked();

    void on_tableInfo_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_checkBoxTableCanEdit_stateChanged(int arg1);

    void on_checkBoxShowRowHeader_clicked(bool checked);

    void on_checkBoxShowColumnHeader_clicked(bool checked);

    void on_checkBoxAlterRowColor_clicked(bool checked);

    void on_radioButtonRowChoice_clicked();

    void on_radioButtonTableChoice_clicked();

private:
    //为某行创建items
    void createItemsARow(int rowNo, QString name, QString sex, QDate birth, QString nation, bool isPM, int score);

private:
    Ui::TableWidgets *ui;
    //定义单元格类型
    enum CellType
    {
        ctName = 1000,
        ctSex,
        ctBirth,
        ctNation,
        ctPartyM,
        ctScore
    };
    enum FieldColNum
    {
        colName,
        colSex,
        colBirth,
        colNatiom,
        colScore,
        colPartyM
    };
    QLabel *labCellIndex; //显示单元格行列号
    QLabel *labCellType;  //显示单元格类型
    QLabel *labStudID;    //显示学号
};

#endif // TABLEWIDGETS_H
