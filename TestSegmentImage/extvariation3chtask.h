#ifndef ExtVariation3ChTask_H
#define ExtVariation3ChTask_H

//#include "abstracttask.h"
#include "dugroupdefect.h"
#include "dutypes.h"

#include <QObject>
#include <vxlib.h>

enum TaskType
{
    //此枚举值涉及到序列化，请不要随意修改
    kCoreTask = 1000, //核心任务
    kCoreTemplate,    //模板匹配

    kBaseTask = 2000, //基本任务
    kBaseLine,        //线任务
    kBaseCircle,      //圆任务
    kBaseArc,         //圆弧任务
    kBaseSegment,     //线段，即轴棒距离

    kTakeTask = 3000,  // take及测量工具
    kTakeAngle,        //量角, 被kMeasureAngle替代,
    kTakeDistance,     //量距离 被kMeasureLength替代
    kTakeSubline,      //辅助线任务,被kMakeSubline替代
    kTakeAffine,       //坐标投射
    kTakeIntersection, //交点 被kMakePoint替代

    kMakeTask = 3100, //替代take
    kMakePoint,       //交点
    kMakeSegment,     //连线，线段
    kMakeMidline,     //中间线
    kMakeMidAngle,    //平分角
    kMakeSubline,     //辅助线

    kExtTask = 4000,  //扩展分析工具
    kExtVariation,    //缺陷检测
    kExtDeformed,     //畸变对比
    kExtBlob,         // blob任务
    kExtGrayDiff,     //双框选灰度差
    kExtRugae,        //折皱检测
    kExtScrew,        //折皱检测
    kExtVariation3Ch, //三通道缺陷检测

    kCombineTask = 5000, //组合工具(>=3的组合)
    kCombineCircle,

    kMeasureTask = 6000, //测量工具
    kMeasureAngle,       //测量角度
    kMeasureArc,         //测量圆
    kMeasureLength,      //测量长度
    kMeasureAffine,      //测量投射

};

class ExtVariation3ChTask : public QObject
{
    Q_OBJECT
public:
    struct Param
    {
        //分割区域参数
        double segma   = 3.0;  //分割因子
        double erosion = 4.0;  //腐蚀因子
        int minarea    = 1000; //分割最小面积
        int channel    = 4;    //分割通道 1:R  2:G  3:B  其他:Gray

        // 三通道检测参数
        int choiceregionnum = 0;   //选择的分割区域
        int difference      = 0;   //差异值，像素单位；替代原来的借用公差方式
        int smallarea       = 200; //最小错误面积
        int lowerlimit[3]{120, 120, 120}; //对比下限
        int upperlimit[3]{180, 180, 180}; //对比上限

        //保留备用
        INT64 model_id; //模板ID,备用
        unsigned long reserved_b{0};
        int reserved_i[16]{0};
        double reserved_lf[16]{0};
    };

    struct SimpParam
    {
        int erode            = 5;    //腐蚀像素
        double error_allowed = 0.01; //简化轮廓时允许的误差
    };

    struct Roi
    {
        //交互相关参数
        float x;
        float y;
        float radius;
        float width;
        float height;
        float angle;
        float angle2;

        //获取roi外形点集
        Stroke stroke()
        {
            std::vector<Pointf> vct;
            float theta = atan2(width, height);
            float l     = hypot(width, height);

            //四个顶点
            Pointf pt;
            float theta1 = theta - (angle) * float(PI) / 180;
            pt.x         = x + l * cos(theta1);
            pt.y         = y + l * sin(theta1);
            vct.push_back(pt);

            theta1 = -theta - (angle) * float(PI) / 180;
            pt.x   = x + l * cos(theta1);
            pt.y   = y + l * sin(theta1);
            vct.push_back(pt);

            theta  = atan2(width, -height);
            theta1 = -theta - (angle) * float(PI) / 180;
            pt.x   = x + l * cos(theta1);
            pt.y   = y + l * sin(theta1);
            vct.push_back(pt);

            theta1 = theta - (angle) * float(PI) / 180;
            pt.x   = x + l * cos(theta1);
            pt.y   = y + l * sin(theta1);
            vct.push_back(pt);

            vct.push_back(vct.at(0));

            return vct;
        }
    };

    ExtVariation3ChTask();
    ~ExtVariation3ChTask();
    enum
    {
        Type = kExtVariation3Ch
    };
    int type() const { return Type; }
    DuGroupDefect *output() { return &m_detected; }

    //    ExtVariation3ChTask *clone() const override;
    bool detect(int mode);

    bool setParam(void *p, int size);
    bool setRoi(void *p, int size);
    //    bool setSimpParam(void *p, int size) override;
    //    bool simplifyRegion() override;
    //    bool getContourRegion();
    bool getSegmentRegion();
    static INT64 imgHandle;
signals:
    void sendEdges(CxPointArray roiArray, CxPointArray segment, CxPointArray CenterArea);

private:
    DuGroupDefect m_detected;

    Param m_param;         //算法参数
    SimpParam m_simpParam; //腐蚀参数
    Roi m_roi;             //交互相关参数
    CxPointArray m_edges;  //检测输出的轮廓
    int m_mode = 0;
    static bool first;
    int segCount = 0;
    std::vector<CxPointArray> m_vctCxPta; //分割区域集合
};

#endif // ExtVariation3ChTask_H
