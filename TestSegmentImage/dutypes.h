#ifndef DUTYPES_H
#define DUTYPES_H

#include <cmath>
#include <opencv2/core.hpp>
#include <vector>

namespace
{
const float EPS = 0.00001f;
const float PAI = std::atan(1.0f) * 4.0f;
} // namespace

struct Pointf
{
    Pointf() : x(0), y(0) {}
    Pointf(float _x, float _y) : x(_x), y(_y) {}
    Pointf(const cv::Point2f &pt) : x(pt.x), y(pt.y) {}

    Pointf operator+(const Pointf &pt) const { return Pointf(x + pt.x, y + pt.y); }
    Pointf operator-(const Pointf &pt) const { return Pointf(x - pt.x, y - pt.y); }
    Pointf operator=(const Pointf &pt)
    {
        x = pt.x;
        y = pt.y;
        return *this;
    }

    operator cv::Point2f()
    {
        cv::Point2f pt;
        pt.x = x;
        pt.y = y;
        return pt;
    }

    float x;
    float y;
};

struct Linef
{
    Pointf pt1;
    Pointf pt2;

    Linef() {}
    Linef(Pointf _pt1, Pointf _pt2) : pt1(_pt1), pt2(_pt2) {}
    Linef(Pointf _pt, float _theta, float _len) : pt1(_pt)
    {
        pt2.x = pt1.x + _len * cos(_theta);
        pt2.y = pt1.y + _len * sin(_theta);
    }

    Linef reverse() { return Linef(pt2, pt1); }

    float angle()
    {
        //线方向角度，跟QLineF.angle返回值相同
        float dx    = pt1.x - pt2.x;
        float dy    = pt1.y - pt2.y;
        float theta = -atan2(dy, dx) + PAI;
        return theta * 180 / PAI;
    }

    float angle1()
    {
        //线反方向角度，
        float dx = pt1.x - pt2.x;
        float dy = pt1.y - pt2.y;
        // return -atan2(-dy, -dx) * 180 / PAI;
        float theta = -atan2(-dy, -dx) + PAI;
        return theta * 180 / PAI;
    }

    float dis() const
    {
        //两点间距离
        float dx = pt1.x - pt2.x;
        float dy = pt1.y - pt2.y;
        return sqrt(dx * dx + dy * dy);
    }

    Pointf mid() const
    {
        //两点中间
        Pointf pt;
        pt.x = (pt1.x + pt2.x) / 2;
        pt.y = (pt1.y + pt2.y) / 2;
        return pt;
    }

    Pointf foot(const Pointf &pt3)
    {
        // pt3相对于本直线的垂足
        float a, b, c;
        equation(a, b, c);

        //垂线方程 bx-ay+d=0;
        float d = a * pt3.y - b * pt3.x;

        //求交点即垂足
        float delta = -(a * a + b * b);
        if (abs(delta) < EPS) return pt3;
        Pointf pt;
        pt.x = (a * c + b * d) / delta;
        pt.y = (b * c - a * d) / delta;
        return pt;
    }

    void adjust2(float l)
    {
        //调整端点2延长或缩短，方向不变
        float dx     = pt2.x - pt1.x;
        float dy     = pt2.y - pt1.y;
        float theta  = atan2(dy, dx);
        float length = hypot(dx, dy);
        length += l;
        pt2.x = pt1.x + length * cos(theta);
        pt2.y = pt1.y + length * sin(theta);
    }

    void adjust1(float l)
    {
        //调整端点1延长或缩短，方向不变
        float dx     = pt1.x - pt2.x;
        float dy     = pt1.y - pt2.y;
        float theta  = atan2(dy, dx);
        float length = hypot(dx, dy);
        length += l;
        pt1.x = pt2.x + length * cos(theta);
        pt1.y = pt2.y + length * sin(theta);
    }

    void equation(float &a, float &b, float &c)
    {
        //求解直线方程参数 ax+bx+c=0;
        a = pt2.y - pt1.y;
        b = pt1.x - pt2.x;
        c = pt2.x * pt1.y - pt1.x * pt2.y;
    }

    bool intersect(Linef l2, Pointf &pt)
    {
        //求解与另一直线的交点
        float a1, b1, c1;
        float a2, b2, c2;
        equation(a1, b1, c1);
        l2.equation(a2, b2, c2);

        float delta = a1 * b2 - a2 * b1;
        if (abs(delta) < EPS) return false;
        pt.x = (b1 * c2 - b2 * c1) / delta;
        pt.y = (a2 * c1 - a1 * c2) / delta;
        return true;
    }

    float angleTo(const Linef &l2)
    {
        //求解与另一直线的夹角
        float dx     = pt2.x - pt1.x;
        float dy     = pt2.y - pt1.y;
        float theta1 = atan2(dy, dx);
        dx           = l2.pt2.x - l2.pt1.x;
        dy           = l2.pt2.y - l2.pt1.y;
        float theta2 = atan2(dy, dx);
        return theta1 - theta2;
    }

    float angleMid(const Linef &l2)
    {
        //求解与另一直线的中间角,夹角小于PAI
        float dx     = pt2.x - pt1.x;
        float dy     = pt2.y - pt1.y;
        float theta1 = atan2(dy, dx);
        dx           = l2.pt2.x - l2.pt1.x;
        dy           = l2.pt2.y - l2.pt1.y;
        float theta2 = atan2(dy, dx);

        if (abs(theta2 - theta1) > PAI)
            return (theta1 + theta2) / 2 + PAI;
        else
            return (theta1 + theta2) / 2;
    }

    void move(const Pointf &src, const Pointf &dst)
    {
        //直线平移
        Pointf delta = dst - src;
        float theta  = atan2(delta.y, delta.x);
        float l      = hypot(delta.x, delta.y);
        float dx, dy;
        dx = l * cos(theta);
        dy = l * sin(theta);
        pt1.x += dx;
        pt1.y += dy;
        pt2.x += dx;
        pt2.y += dy;
    }

    Linef createSubline(const Pointf &ori, float angle)
    {
        //根据原点/角度(度)生成一条新的线，新线长度与本线等长

        float dx    = pt2.x - pt1.x;
        float dy    = pt2.y - pt1.y;
        float theta = atan2(dy, dx);

        //端点
        Linef subline;
        float l;
        dx            = pt1.x - ori.x;
        dy            = pt1.y - ori.y;
        l             = hypot(dx, dy);
        float theta1  = theta + PAI - angle * PAI / 180;
        subline.pt1.x = ori.x + l * cos(theta1);
        subline.pt1.y = ori.y + l * sin(theta1);

        dx            = pt2.x - ori.x;
        dy            = pt2.y - ori.y;
        l             = hypot(dx, dy);
        float theta2  = theta - angle * PAI / 180;
        subline.pt2.x = ori.x + l * cos(theta2);
        subline.pt2.y = ori.y + l * sin(theta2);

        return subline;
    }

#if 0
    Linef createMidAngle(const Linef &l2, Linef &out)
    {
        //确定顶点
        Pointf vertex;
        bool ok = intersect(l2, vertex);

        //确定三角形的另两个顶点
        Pointf pt0, pt1;
        Linef l0, l1;
        l0  = Linef(vertex, pt1);
        l1  = Linef(vertex, pt2);
        pt0 = l0.dis() > l1.dis() ? l0.pt2 : l1.pt2;
        l0  = Linef(vertex, l2.pt1);
        l1  = Linef(vertex, l2.pt2);
        pt1 = l0.dis() > l1.dis() ? l0.pt2 : l1.pt2;

        //确定线长
        float len = (dis() + l2.dis()) / 2;

        //求解与另一直线的中间角,
        float dx, dy;
        float theta1, theta2;
        dx     = pt2.x - pt1.x;
        dy     = pt2.y - pt1.y;
        theta1 = atan2(dy, dx);
        dx     = l2.pt2.x - l2.pt1.x;
        dy     = l2.pt2.y - l2.pt1.y;
        theta2 = atan2(dy, dx);

        //夹角小于PAI
        float theta;
        if (abs(theta2 - theta1) > PAI)
            theta = (theta1 + theta2) / 2 + PAI;
        else
            theta = (theta1 + theta2) / 2;

        Linef line;
        line.pt1 = vertex;
        pt2.x    = pt1.x + len * cos(theta);
        pt2.y    = pt1.y + len * sin(theta);
        return line;
    }
#endif
};

typedef std::vector<Pointf> Stroke; //点集，
// typedef std::vector<Stroke> Contour; //轮廓，

#endif // DUTYPES_H
