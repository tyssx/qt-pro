#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QDockWidget>
#include <QStackedWidget>
#include <QFrame>

// 一个具体的应用;例如 共聚焦激光测量
class WidgetTDisplay;
class AppConLaser;
class AppSensor;
//=================================
namespace Ui {
class MainWindow;
}
// 作为主窗口 ,来构建 一个应用的界面
// 界面上接口,加载对应的 模块(含界面和应用)即可
// 方案 1  所有对ui的操作 都在 mainwindow 里 ;

// 方案 2  把私有 mainwindow::ui指针 开放一个接口getMainUi(); 在 自定义类里访问 ui ;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 基础数据
    Ui::MainWindow *ui;

    // ver1.0  直接用qtcreater 创建界面
    // 计划用代码写公用接口界面,

    //程序模板1 左侧显示;右侧不同功能页; 最右边功能按钮

    //程序模板2  一键测量 有多个测量目标

    //程序模板3  流程编程



    //=== 共聚焦激光应用 ================
    AppConLaser *pLaser= nullptr;

    //====================================

    AppSensor *pSensor = nullptr;

public:

     //基础接口
     Ui::MainWindow * getMainUi();

     //程序模板1 通用接口 开始
     void addUiTDisplay(QWidget * pWidget);
     void addUiTFunBtns(QWidget *pWidget);
     void addUiTabInfo(QWidget *pWidget);

     // Stacked多页界面 ;功能页面,
     void addUi_StackedPage(QWidget * w);//加入一页
     void setUi_CurStPage(QWidget * w);//设置当前页

     //显示功能按钮区域
     void showUiTFunBtns(bool bShow);

     void showUiTDisplay(bool bShow); //不要 显示页
     void showUiTabInfo(bool bShow);  //不要 信息页
     // 通用接口结束
     void writeUiPosSettings();
     void readUiPosSettings();

private slots:
     void on_actionEditModel_triggered();
};

#endif // MAINWINDOW_H
