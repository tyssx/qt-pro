#ifndef TREEWIDGETS_H
#define TREEWIDGETS_H

#include <QLabel>
#include <QMainWindow>
#include <QTreeWidgetItem>

namespace Ui
{
class TreeWidgets;
}

class TreeWidgets : public QMainWindow
{
    Q_OBJECT

public:
    explicit TreeWidgets(QWidget *parent = nullptr);
    ~TreeWidgets();

private slots:
    void on_actionAddFolder_triggered();

    void on_actionAddFiles_triggered();

    void on_treeFiles_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_actionDeleteItem_triggered();

    void on_actionScanItems_triggered();

    void on_actionZoomFitH_triggered();

    void on_actionZoomFitW_triggered();

    void on_actionZoomIn_triggered();

    void on_actionZoomOut_triggered();

    void on_actionZoomRealSize_triggered();

    void on_actionDockVisible_toggled(bool arg1);

    void on_actionDockFloat_triggered(bool checked);

    void on_dockWidget_visibilityChanged(bool visible);

    void on_dockWidget_topLevelChanged(bool topLevel);

private:
    void initTree();
    void addFolderItem(QTreeWidgetItem *parItem, QString dirName);  //添加目录
    QString getFinalFolderName(const QString &fullPathName);        //提取目录名称
    void addImageItem(QTreeWidgetItem *parItem, QString aFilename); //添加图片
    void displayImage(QTreeWidgetItem *item);                       //显示一个图片节点的图片
    void changeItemCaption(QTreeWidgetItem *item);                  //遍历改变节点标题

private:
    Ui::TreeWidgets *ui;
    enum treeItemType //节点类型
    {
        itTopItem = 1001,
        itGroupItem,
        itImageItem
    };
    enum treeColNum //目录树列编号
    {
        colItem,
        colItemType
    };
    QLabel *labFileName; //用于状态栏文件名显示
    QPixmap curPixmap;   //当前图像
    float pixRatio;      //图片缩放比例
};

#endif // TREEWIDGETS_H
