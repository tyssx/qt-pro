#include "sportcontrol.h"
#include <QDebug>
#include <QTimer>

SportControl::SportControl(QObject *parent) : QObject(parent)
{

}

void SportControl::singleAxisSportSlot(void *g_handle,int iaxis, int distance)
{
    int run_state = 0; //轴运动状态
    ZAux_Direct_Singl_Move(g_handle,iaxis,distance); //参数：链接标识 轴号 距离float
    do
    {
        //判断当前轴运动状态，状态 IDLE 返回只有 0，-1 两种值，0 表示当前轴正在运动中，-1 表示当前轴没有运动，
        ZAux_Direct_GetIfIdle(g_handle,0,&run_state); //读取轴 0 运动状态，0-运动，-1 停止
    }
    while (run_state == 0);
}

/*imode 回零模式 */
void SportControl::leftRightOriginSlot(void *g_handle,int imode)
{
    //轴棒检测的原点IN为24
    ZAux_Direct_SetDatumIn(g_handle,0,24); //设置 IN(24)为轴 0 的原点信号
    ZAux_Direct_SetSpeed (g_handle,0,200); //设置轴 0 速度为 200units/s
    ZAux_Direct_SetCreep(g_handle,0,20); //设置轴 0 爬行速度为 20units/s
    ZAux_Direct_Singl_Datum(g_handle,0,imode);
}

void SportControl::autoSportSlot(void *g_handle)
{
    //轴棒检测的原点IN为24
    ZAux_Direct_SetDatumIn(g_handle,0,24); //设置 IN(24)为轴 0 的原点信号
    ZAux_Direct_SetSpeed (g_handle,0,400); //设置轴 0 速度为 200units/s
    ZAux_Direct_SetCreep(g_handle,0,20); //设置轴 0 爬行速度为 10units/s
    ZAux_Direct_Singl_Datum(g_handle,0,4); //轴 0 按模式 4 回零，负向 2 次回零。

    int run_state = 0; //轴运动状态
    do
    {
        //判断当前轴运动状态，状态 IDLE 返回只有 0，-1 两种值，0 表示当前轴正在运动中，-1 表示当前轴没有运动，
        ZAux_Direct_GetIfIdle(g_handle,0,&run_state); //读取轴 0 运动状态，0-还在运动，-1 运动停止
    }
    while (run_state == 0);

    ZAux_Direct_SetSpeed (g_handle,0,300); //设置轴 0 速度为 200units/s
    for(int i=0;i<8;i++)
    {
        //向右运动400 4cm
        run_state = 0; //轴运动状态
        ZAux_Direct_Singl_Move(g_handle,0,400); //参数：链接标识 轴号 距离float
        do
        {
            //判断当前轴运动状态，状态 IDLE 返回只有 0，-1 两种值，0 表示当前轴正在运动中，-1 表示当前轴没有运动，
            ZAux_Direct_GetIfIdle(g_handle,0,&run_state); //读取轴 0 运动状态，0-还在运动，-1 运动停止
        }
        while (run_state == 0);

        //顺时针转
        run_state = 0;
        ZAux_Direct_Singl_Move(g_handle,1,600); //参数：链接标识 轴号 距离float
        do
        {
            //判断当前轴运动状态，状态 IDLE 返回只有 0，-1 两种值，0 表示当前轴正在运动中，-1 表示当前轴没有运动，
            ZAux_Direct_GetIfIdle(g_handle,1,&run_state); //读取轴 0 运动状态，0-运动，-1 停止
        }
        while (run_state == 0);
    }
}


//void SportControl::stopAutoSportSlot(void *g_handle)
//{
//    QTimer *timer = new QTimer(this);
//    timer->start(200);
//    int n = 0; //lambda表达式中，捕获的变量n默认是不可变的,需要加上mutable。
//    connect(timer,&QTimer::timeout,this,[=]()mutable{
//        int run_state = 1; //轴运动状态
//            qDebug() << "n = " << n;
//        ZAux_Direct_GetIfIdle(g_handle,0,&run_state);
//        if(run_state == 0)
//        {
//            ZAux_Direct_Singl_Cancel(g_handle, 0, 2);
//                qDebug() << "0 cancel";
//        }
//        ZAux_Direct_GetIfIdle(g_handle,1,&run_state);
//        if(run_state == 0)
//        {
//            ZAux_Direct_Singl_Cancel(g_handle, 1, 2);
//                qDebug() << "1 cancel";
//        }
//        if(n++ == 16)
//        {
//            timer->stop();
//            qDebug() << " timer->stop";
//        }
//    });
//}
