#ifndef IMAGEDEAL_H
#define IMAGEDEAL_H

#include <QObject>
#include <QImage>
#include <QThread>
#include "Detection.h"
#include "Blob.h"
#include "Basic.h"

class ImageDeal : public QObject
{
    Q_OBJECT
public:
    explicit ImageDeal(QObject *parent = nullptr);

    struct Parameter
    {
        double e1;   //黑色容差
        double e3;   //白色容差，值小时检测的内容区域比较多，值大时检测的内容区域比较少
        int blackff;     //是否测黑，是1，否0
        int whiteff;     //是否测白，是1，否0
        int vArea;
    };

signals:
    void singleFinishSingnal(long mSecTime,QImage imgResult,int mCount);
//    void allImgFinishSingnal();

public slots:
//    void grabStartSlot();
    void imageDealSlot(QImage image,ImageDeal::Parameter param,int num);
//    void grabStopSlot(int n);

private:
    int width;
    int height;
    int size;
    uchar *Imgp;
    uchar *Imgp2RGB;
    uchar *ImgpMask;
    uchar * Imgp2Result;
    uchar * Imgp2Result2;
    uchar * Imgp2Result2edge;
    double * Canca;

    static int count;
    static int m_Num;
    int GetPointFrom8BitsImage(QImage InImage, unsigned char *InImagePoint, const int ImageWidth,const int ImageHeight);
    QImage RgbImageToGrayImage(QImage RgbImage);
    QImage Get24BitsImageImageFromArray(unsigned char *OriginalData,const int ImageWidth, const int ImageHeight);
    QImage GetImageFromArray(unsigned char *OriginalData,const int ImageWidth, const int ImageHeight);
};

#endif // IMAGEDEAL_H
