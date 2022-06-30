#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "camlightdlg.h"
#include "imageform.h"

#include <QCloseEvent>
#include <QMainWindow>
#include <QtCharts>

using namespace QtCharts;

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    struct MaxAppraise
    {
        int value;
        int lwhite;
        int lred;
        int lgreen;
        int lblue;
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static int MaxFunc(int numoflights, const int input[], void *optionrpara);
    //    static int MaxFunc(int numoflights, const int input[], int optionrpara[]);
    QImage getImage() { return m_image; }

protected:
    virtual void closeEvent(QCloseEvent *event);

private:
    void createChart();              //创建图表
    void appraiseDeal(int output[]); //最优处理函数

signals:
    void lightValueSignal(int, int, int, int, int);
    void lightDealEndSignal();
    void getImageSignal(QImage image);

public slots:
    void grabImageUI(QImage image);
    void slotStartAutoLight(ImageForm::SRoi sroi);

private slots:
    void on_action_set_triggered();

    void on_action_saveImage_triggered();

private:
    Ui::MainWindow *ui;
    CStorageInterface *m_store = nullptr;
    camLightDlg *m_camLightDlg = nullptr;
    QSettings *m_settings      = nullptr;

    ImageForm *m_ImageForm;
    QDockWidget *dockWidget;
    QChartView *chartView;
    QLineSeries *curSeries; //当前序列
    QValueAxis *curAxis;    //当前坐标轴
    QLabel *curlabel;       //实时评价值状态栏显示
    QLabel *maxlabel;       //最优评价值状态栏显示
                            //    QLabel *lightlabel;     //实时光源值状态栏显示

    MaxAppraise m_MaxAppraise;

    //新增
    QImage m_image;
    int m_left, m_top, m_width, m_height;
    static int num;

    QTimer *timer;
    QComboBox *m_ComboBox;
};

#endif // MAINWINDOW_H
