#include "common.h"
#include "mealbuilder.h"
#include "vld.h"
#include "widget.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QPointer>
#include <iostream>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

using namespace std;

struct Pointf
{
    Pointf() : x(0), y(0) {}
    Pointf(float _x, float _y) : x(_x), y(_y) {}

    Pointf operator+(const Pointf &pt) const { return Pointf(x + pt.x, y + pt.y); }
    Pointf operator-(const Pointf &pt) const { return Pointf(x - pt.x, y - pt.y); }
    Pointf operator=(const Pointf &pt)
    {
        x = pt.x;
        y = pt.y;
        return *this;
    }
    float x;
    float y;
};

Pointf toReal(const Pointf &pt_pixel)
{
    cout << "pt_pixel " << pt_pixel.x << " " << pt_pixel.y << endl;
    return {pt_pixel.x + 2, pt_pixel.y + 2};
}
void toReal(const float &length_pixel, const Pointf &datum_pixel)
{
    Pointf pt0 = toReal({datum_pixel.x - length_pixel / 2, datum_pixel.y});
    Pointf pt1 = toReal({datum_pixel.x + length_pixel / 2, datum_pixel.y});
    cout << "pt0 " << pt0.x << " " << pt0.y << endl;
    cout << "pt1 " << pt1.x << " " << pt1.y << endl;
}

int &ref()
{
    int i  = 3;
    int &j = i;
    return j;
}

void SendCmd(int nCMD, int nCH, int nData)
{
    char *nSendBuf = new char[20];

    int nIndex    = 0;
    int nSumCheck = 0;

    // 数据头
    nSendBuf[nIndex] = (char)'$';
    nSumCheck        = nSendBuf[nIndex];
    nIndex++;

    // 指令号
    nSendBuf[nIndex] = (char)((nCMD % 256) + '0');
    nSumCheck ^= nSendBuf[nIndex];
    nIndex++;

    // 通道号或者信息1
    nSendBuf[nIndex] = (char)((nCH % 256) + '0');
    nSumCheck ^= nSendBuf[nIndex];
    nIndex++;

    // 固定填充值
    nSendBuf[nIndex] = (char)'0';
    nSumCheck ^= nSendBuf[nIndex];
    nIndex++;

    // 数据高位
    char nDataH = (char)((nData % 256) / 16);
    if (nDataH < 10)
        nDataH += (char)'0';
    else
        nDataH += (char)'A' - 10;
    nSendBuf[nIndex] = nDataH;
    nSumCheck ^= nSendBuf[nIndex];
    nIndex++;

    // 数据低位
    char nDataL = (char)((nData % 256) % 16);
    if (nDataL < 10)
        nDataL += (char)'0';
    else
        nDataL += (char)('A' - 10);
    nSendBuf[nIndex] = nDataL;
    nSumCheck ^= nSendBuf[nIndex];
    nIndex++;

    // 校验高位
    nDataH = (char)(nSumCheck / 16);
    if (nDataH < 10)
        nDataH += (char)'0';
    else
        nDataH += (char)('A' - 10);
    nSendBuf[nIndex++] = nDataH;

    // 校验低位
    nDataL = (char)(nSumCheck % 16);
    if (nDataL < 10)
        nDataL += (char)'0';
    else
        nDataL += (char)('A' - 10);
    nSendBuf[nIndex++] = nDataL;

    QByteArray byte(nSendBuf, nIndex);
    qDebug() << "byte" << byte.toHex();
}

void csss()
{
    static int aa;
}
class ZX
{
public:
    int a() { return _a; }
    virtual int b() { return 0; }
    virtual int c() { return 0; }

private:
    char _a = 'a';
    //    short _b;
    //    int _c;
};

class ZC : public ZX
{
public:
    int b() { return 1; }
};

class Test1
{
    virtual void func1() {}
    virtual void func2() {}

public:
    virtual ~Test1();
};

class Test2
{
    virtual void func2() {}
};

class SonOfTest1 : public Test1
{
    virtual void func2() {}
    virtual void func3() {}
};

class SonOfTest1Test2 : public Test1, public Test2
{
    virtual void func4() {}
};

class Test
{
public:
    Test() { cout << "Test的构造函数..." << endl; }
    ~Test() { cout << "Test的析构函数..." << endl; }

    int getDebug() { return this->debug; }

private:
    int debug = 20;
};

class Complex
{
public:
    /*explicit*/ Complex(int a = 0, int b = 0)
    {
        m_a = a;
        m_b = b;
    }

    bool operator==(Complex cpx) { return (m_a == cpx.m_a && m_b == cpx.m_b) ? true : false; }

private:
    int m_a;
    int m_b;
};

class Complex1
{
public:
    Complex1(int a = 0) { m_a = a; }

    bool operator==(Complex1 cpx) { return (m_a == cpx.m_a) ? true : false; }

private:
    int m_a;
};

class A
{
public:
    void fun() { cout << "base fun" << endl; }
};

class B : public A
{
public:
    void fun() { cout << "son fun" << endl; }
    void fun(int i) { cout << "son fun" << i << endl; }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    QPointer<QLabel> label(new QLabel("hello"));

    A aa;
    aa.fun();

    B bb;
    bb.fun();
    bb.fun(2);

    std::cout << "Hello World!\n";

    Complex complex(4, 5);
    if (complex == 4) { cout << "same" << endl; }
    else
    {
        cout << "not same" << endl;
    }

    Complex1 comp1(5);
    if (comp1 == 6) { cout << "same" << endl; }
    else
    {
        cout << "not same" << endl;
    }

    //    MealBuilder *mealBuidler = new MealBuilder();
    //    Meal *vegMeal            = mealBuidler->prepareVegMeal();
    //    cout << "veg meal" << endl;
    //    vegMeal->showFoods();
    //    cout << "Total Cost: " << vegMeal->getCost() << endl;
    //    Meal *nonVegMeal = mealBuidler->prepareNonMeal();
    //    nonVegMeal->showFoods();

    //    delete label;
    //    delete mealBuidler;
    //    delete nonVegMeal;
#if 0
    tem();

    num = 0;
    qDebug() << "num..." << num;

    ZX zx;
    qDebug() << "sizeof()" << sizeof(zx);

    cout << "sizeof(Test1):" << sizeof(Test1) << endl;

    cout << "sizeof(Test2):" << sizeof(Test2) << endl;

    cout << "sizeof(SonOfTest1):" << sizeof(SonOfTest1) << endl;

    cout << "sizeof(SonOfTest1Test2):" << sizeof(SonOfTest1Test2) << endl;

    ZC zc;
    qDebug() << "zc.a" << zc.a() << zc.b();

    ZX &zxz = zc;
    qDebug() << "zxz.a" << zxz.a() << zxz.b();
    ZX *zxx = new ZC;
    qDebug() << "zxx->a" << zxx->a() << zxx->b();
    //    Pointf pt0;
    //    Pointf pt1(10, 20);
    //    Pointf pt2 = pt0 - pt1;
    //    std::cout << "pt2 " << pt2.x << " " << pt2.y << std::endl;
    //    float length = 50;
    //    toReal(length, pt2);
    //    int &rr = ref();
    //    std::cout << "rr = " << rr;
    //    QMap<int, QString> map;
    //    map.insert(0, "这是A");
    //    map.insert(1, "这是B");
    //    map.insert(2, "这是C");
    //    map.insert(0, "这是D");
    //    map.insert(1, "这是E");
    //    map.insert(2, "这是F");
    //    map.insert(3, "这是G");

    //    qDebug() << "map" << map.size() << map;
    /* int x = 0;
     //    qint16 x = 0;
     QByteArray byte;
     byte.append('\xff');
     byte.append('\xff');
     byte.append('\xc6');
     byte.append('\xdb');
     //    byte.append('\xe8');
     //    byte.append('\x90');
     QDataStream ds(byte);
     ds >> x;

     qDebug() << "x=" << x;
     //    int count = (ad[2] - 0x0E) / 4;
     QString ret;
     for (int i = 0; i < byte.count(); ++i)
     { ret.append(QString("0x%1,").arg((quint8)byte.at(i), 2, 16, QLatin1Char('0')).toUpper()); }

     qDebug() << ret;

     qDebug() << "byte" << byte;*/

    int value   = 56;
    int port    = 2;
    QString str = QString("$3%1%2").arg(port).arg(value, 3, 10, QLatin1Char('0'));
    //    int unit         = value / 1 % 10;
    //    int ten          = value / 10 % 10;
    //    int hundred      = value / 100 % 10;
    //    QByteArray unitByte    = QString::number(unit, 16).toUpper().toUtf8();
    //    QByteArray tenByte     = QString::number(ten, 16).toUpper().toUtf8();
    //    QByteArray hundredByte = QString::number(hundred, 16).toUpper().toUtf8();
    //    QString str = QString("$3%1%2%3%4").arg(port).arg(hundred).arg(ten).arg(unit);
    qDebug() << "str" << str;

    QByteArray byte = str.toUtf8();
    //    byte.append('\x24');
    //    byte.append('\x33');
    //    byte.append('\x32'); //通道
    //    byte.append('\x30');
    //    byte.append(vByte.at(0));
    //    byte.append(vByte.at(1));
    //    QByteArray bXor;
    //    uchar bXor       = byte[0] ^ byte[1] ^ byte[2] ^ byte[3] ^ byte[4] ^ byte[5];
    char cXor = byte.at(0);
    for (int i = 1; i < byte.size(); i++) { cXor ^= byte.at(i); }
    QByteArray xByte = QString::number(cXor, 16).toUpper().toUtf8();
    byte += xByte;
    qDebug() << "xByte" << xByte.toHex();
    //    byte.append(xByte.at(0));
    //    byte.append(xByte.at(1));

    qDebug() << "bytA" << byte.toHex();

    SendCmd(3, 2, 56);
    SendCmd(8, 2, 0);

    //    2433323033393146 57
    //    2433323033413637 58
#endif
    return a.exec();
}
