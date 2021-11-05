#ifndef APPCONLASER_H
#define APPCONLASER_H

#include <QObject>
#include <QButtonGroup>

#if _MSC_VER >= 1600
# pragma execution_character_set("utf-8")
#endif

//所有和界面相关的
class MainWindow;

class WidgetTDisplay; //区域: 显示
class WidgetTabInfo;

class WidgetTFunBtns; //区域: 工具栏

// 区域: 功能页 各功能页
class PageRun;
class PageProject;
class PageImage;

class WidgetSensorDisplay;




//==================================
class AppConLaser : public QObject
{
    Q_OBJECT
public:
    explicit AppConLaser(QObject *parent = nullptr);

    // 重写一个构造函数, 从主窗口直接构造
    explicit AppConLaser(QWidget *w, QObject *parent = nullptr);

    ~AppConLaser();
    //===================================
    typedef struct struUiGroup
    {
         WidgetTDisplay *pUiTDisplay;
         WidgetTFunBtns *pUiTFunBtns;
         WidgetTabInfo*  pUiTabInfo;

         PageRun      *pUiPageRun;
         PageProject  *pUiPageProject;

         PageImage    *pUiPageImage;

    }struUiGroup;

     struUiGroup  groupConLaserUi;

private:
    //主窗口
    MainWindow * pMain= nullptr;

public:
    void setMainWindow(MainWindow *w);
    bool createUi();

    // 共聚焦激光的界面
    void addUiTDisplay(QWidget * w); //加载 显示界面.可以是图像,相机,实时图形
    void addUiTabInfo(QWidget * w);  //加载 信息页.

    void addUiTFunBtns(QWidget * w);  //加载 功能按钮.

    void addUiTFunPages(QWidget * w); //加载  功能页
    void setUiTFunCurPage(QWidget * w);//设置 当前功能页

    //界面显示
    void showUiTFunBtns(bool bShow);//显示或隐藏功能按钮区

     void showUiTDisplay(bool bShow); //显示或者隐藏相机显示区

     void showUiTabInfo(bool bShow); //显示或隐藏 信息也

public:
  signals:

public slots:
     //切换页面
     void changePage(int nActivePage);

};

#endif // APPCONLASER_H
