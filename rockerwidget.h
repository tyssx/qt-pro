#ifndef ROCKERWIDGET_H
#define ROCKERWIDGET_H

#include "device/devicemanager.h"

#include <QWidget>

class RockerWidget : public QWidget
{
    Q_OBJECT

public:
    enum AxisType
    {
        loading,
        unloading
    };
    explicit RockerWidget(QWidget *parent = nullptr);
    ~RockerWidget() Q_DECL_OVERRIDE;
    void init(const DeviceManager *device, int axisType);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private:
    const DeviceManager *m_device;
    CMotionCtrlDev *m_motion; //引用至控制器
    bool m_flag;              // mouse左键按下轴标志
    qreal m_radius;           //工作半径
    qreal m_rHand;            //摇把头半径
    QPointF m_pos;            //轴当前位置
    QPointF m_centre;         //轴中心点
    QLineF m_line;            // 轴摇杆指示线
    qreal w;                  // 轴宽

    int m_timeId = 0; //定时器ID
    //交互区域 上下料轴：0摇杆，12上下
    QPainterPath m_path[3];
    bool isHover = false; //是否悬停

    CMotionCtrlDev::SSpeedSettings m_speed;

    int m_axis;        //轴号
    int m_axisType;    //轴类型
    int m_maxSpeed;    //摇杆最大速度
    double m_smallDis; //单次微调距离

signals:
    void rockerTriggered();
};

#endif // ROCKERWIDGET_H
