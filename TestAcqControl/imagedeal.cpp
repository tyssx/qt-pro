#include "imagedeal.h"

#include <QDebug>

int ImageDeal::m_Num = -1;
int ImageDeal::count = -1;

ImageDeal::ImageDeal(QObject *parent) : QObject(parent)
{
    count = 0;
}

//void ImageDeal::grabStartSlot()
//{

//}

void ImageDeal::imageDealSlot(QImage image,ImageDeal::Parameter param,int num)
{
    qDebug()<< "<----imageDealSlot work,thread id = " << QThread::currentThreadId() << "---->";
//    qDebug()<< param.e1;
//    qDebug()<< param.e3;
//    qDebug()<< param.blackff;

    width = image.width();
    height = image.height();
    size = width * height;
    qDebug() << "width = " << image.width() << " height = " << image.height() << " size = " << size ;

    Imgp = new uchar[size]();       //获取原始图像的灰度数据
    Imgp2RGB = new uchar[size * 3](); //转换为RGB的数据
    ImgpMask = new uchar[size]();  //掩膜数据
    Imgp2Result = new uchar[size]();
    Imgp2Result2 = new uchar[size]();
    Canca = new double[size]();
    Imgp2Result2edge = new uchar[size]();

    memset(Imgp,0,sizeof(Imgp));
    memset(Imgp2RGB,0,sizeof(Imgp2RGB));
    memset(ImgpMask,0,sizeof(ImgpMask));
    memset(Canca,0,sizeof(Canca));
    memset(Imgp2Result,0,sizeof(Imgp2Result));
    memset(Imgp2Result2,0,sizeof(Imgp2Result2));
    memset(Imgp2Result2edge,0,sizeof(Imgp2Result2edge));
//    qDebug() << "Imgp" << Imgp;

    //获取图像的灰度数据到Imgp
    GetPointFrom8BitsImage(image, Imgp, width, height);
//    QFile file("D:/e.txt");
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
//    file.write(reinterpret_cast<char *>(Imgp));
//    file.close();

    //原始的灰度数据转换为RGB数据
    for (int i = 0; i < size; ++i)
    {
        for (int k = 0; k < 3; k++)
        {
            Imgp2RGB[i * 3 + k] = Imgp[i];
        }
    }

    //初始化掩膜数据
    for (int i = 0; i < size; ++i)
    {
        ImgpMask[i] = 255; //白
    }

    //选择图像需要处理的区域
    QRect roi(50,50,width-100,height-100);  //在左，上坐标增大100时，宽度高度要减少200
    qDebug() << roi.left() << roi.top()<<roi.width()<< roi.height();

    for (int i = roi.left(); i < roi.left() + roi.width(); i += 2)
    {
        Imgp2RGB[((roi.top())*width + i) * 3 + 1] = 255;
        Imgp2RGB[((roi.top() + roi.height())*width + i) * 3 + 1] = 255;
    }
    for (int i = roi.top(); i < roi.top() + roi.height(); i += 2)
    {
        Imgp2RGB[(i*width + roi.left()) * 3 + 1] = 255;
        Imgp2RGB[(i*width + roi.left() + roi.width()) * 3 + 1] = 255;
    }

    //输入参数，开始图像算法处理，生成的图片呈黑白
//    qDebug() << "roistart";
    clock_t roistart = clock();
    detect_mean_roi_mask(Imgp,width,height,ImgpMask, Imgp2Result, Canca, roi.left(), roi.top(), roi.width(), roi.height(), param.e1, param.e3, param.blackff, param.whiteff);
    clock_t roiend = clock();
//    qDebug() << "roiend";
    long mSecTime = roiend - roistart;

    //保存detect_mean_roi_mask生成的图像
//    QImage xx = GetImageFromArray(Imgp2Result, width, height);
//    xx.save("D:\\3.jpg");
//    ui->label->setPixmap(QPixmap::fromImage(xx).scaled(ui->label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
//    update();

#if 0 //耗时12s去掉
//    clock_t roistart1 = clock();
    //连通域大小过滤
    BwAreaMinAndMaxOpen(Imgp2Result, Imgp2Result2, width, height, param.vArea, 10000000);
//    clock_t roiend1 = clock();
//    long mSecTime1 = roiend1 - roistart1;
//    qDebug() << "BwAreaMinAndMaxOpen time" << mSecTime1;
//    clock_t roistart2 = clock();
    //连通域边界
    ImageEdge(Imgp2Result2, Imgp2Result2edge, width, height);
#else
//    clock_t roistart2 = clock();
    ImageEdge(Imgp2Result, Imgp2Result2edge, width, height);
#endif
//    clock_t roiend2 = clock();
//    long mSecTime2 = roiend2 - roistart2;
//    qDebug() << "ImageEdge time" << mSecTime2;

    bool ResultShow = true;
    if (ResultShow)
    {
        for (int i = 0; i < size; ++i)
        {
            if (Imgp2Result2edge[i])
            {
                Imgp2RGB[i * 3] = 255;
                Imgp2RGB[i * 3 + 1] = 0;
                Imgp2RGB[i * 3 + 2] = 0;
            }
        }
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            if (Imgp2Result2[i])
            {
                Imgp2RGB[i * 3] = 255;
                Imgp2RGB[i * 3 + 1] = 0;
                Imgp2RGB[i * 3 + 2] = 0;
            }
        }
    }

//    clock_t roistart3 = clock();
    //保存图像处理结果
    QImage imgResult;
    imgResult = Get24BitsImageImageFromArray(Imgp2RGB, width, height);
//    clock_t roiend3 = clock();
//    long mSecTime3 = roiend3 - roistart3;
//    qDebug() << "Get24BitsImageImageFromArray time" << mSecTime3;

    qDebug() << "count" << count << "num" << num;
    if(num == count)
    {
        qDebug() << "----imagedeal emit singleFinishSingnal count----" << count;
        emit singleFinishSingnal(mSecTime,imgResult,count);
    }
    count++;

    delete[] Imgp2Result2edge;
    delete[] Imgp2Result2;
    delete[] Imgp2Result;
    delete[] Canca;
    delete[] ImgpMask;
    delete[] Imgp2RGB;
    delete[] Imgp;

}

//void ImageDeal::grabStopSlot(int n)
//{
//    m_Num = n;
//    qDebug() << "++++++Recv grabStopSlot m_Num ++++++" << m_Num;
//}

int ImageDeal::GetPointFrom8BitsImage(QImage InImage, unsigned char *InImagePoint, const int ImageWidth,const int ImageHeight)
{

    if ((ImageWidth <= 0) || (ImageHeight <= 0))
    {
        return 1;
    }
    InImage = RgbImageToGrayImage(InImage);
    memset(InImagePoint, 0, ImageWidth*ImageHeight * sizeof(unsigned char));
    unsigned char* ImageData = InImage.bits();
    //若ImageWidth不是4的倍数，会自动添加字节，使之对齐到4的倍数
    int RealWidth = InImage.bytesPerLine();

    unsigned char* BackUp = ImageData;

    for (int i = 0; i < ImageHeight; i++)
    {
        for (int j = 0; j < ImageWidth; j++)
        {
            InImagePoint[i*ImageWidth + j] = *ImageData;
            ImageData += 1;
        }
        ImageData = BackUp + RealWidth * (i + 1);
    }
    return 0;
}

QImage ImageDeal::RgbImageToGrayImage(QImage RgbImage)
{

    const int ImageDepth = RgbImage.depth();
    if (ImageDepth == 8)
    {
        //   qDebug()<<"Image is already gray! Conversion stopped!";
        return RgbImage.copy();
    }
    QSize ImageSize = RgbImage.size();
    QImage GrayImage = QImage(ImageSize, QImage::Format_Indexed8);
    int ImageWidth = ImageSize.width();
    int ImageHeight = ImageSize.height();
    unsigned char * RgbImageData = RgbImage.bits();
    unsigned char * GrayImageData = GrayImage.bits();
    //若width不是4的倍数，会自动添加字节，使之对齐到4的倍数
    int RgbRealWidth = RgbImage.bytesPerLine();
    int GrayRealWidth = GrayImage.bytesPerLine();
    unsigned char * RgbBackUp = RgbImageData;
    unsigned char * GrayBackUp = GrayImageData;
    for (int i = 0; i < ImageHeight; i++)
    {
        for (int j = 0; j < ImageWidth; j++)
        {
            unsigned char RValue = *(RgbImageData + 2);
            unsigned char GValue = *(RgbImageData + 1);
            unsigned char BValue = *(RgbImageData);
            *GrayImageData = static_cast<unsigned char>((299 * RValue + 587 * GValue + 114 * BValue) / 1000);
            RgbImageData += 4;
            GrayImageData++;
        }
        RgbImageData = RgbBackUp + RgbRealWidth * (i + 1);
        GrayImageData = GrayBackUp + GrayRealWidth * (i + 1);
    }
    for (int i = 0; i < 256; i++)
    {
        GrayImage.setColor(i, qRgb(i, i, i));
    }
    return GrayImage;
}

QImage ImageDeal::Get24BitsImageImageFromArray(unsigned char *OriginalData,const int ImageWidth, const int ImageHeight)
{


    QImage OutImage(ImageWidth, ImageHeight, QImage::Format_RGB32);//创建同样大小深度为32(即每个像素占8位)的空白图

    for (int i = 0; i < ImageHeight; ++i)
    {
        for (int j = 0; j < ImageWidth; ++j)
        {
            int nTempIndex = i * ImageWidth + j;
            QRgb rgbTemp = qRgb(OriginalData[nTempIndex * 3],
            OriginalData[nTempIndex * 3 + 1],
            OriginalData[nTempIndex * 3 + 2]); // 0xffbd9527
            OutImage.setPixel(j, i, rgbTemp);
        }
    }
    //  unsigned char * OutData = OutImage.bits();
    //  for (int i = 0; i < ImageWidth*ImageHeight; ++i)
    //  {
    //   *(OutData) = OriginalData[i*3 + 2];
    //   *(OutData + 1) = OriginalData[i*3 + 1];
    //   *(OutData + 2) = OriginalData[i*3 + 0];
    // //   *(OutData + 3) = ZC_MaxGray;
    //   *(OutData) += 4;
    //  }
    return OutImage;

}


QImage ImageDeal::GetImageFromArray(unsigned char *OriginalData,const int ImageWidth, const int ImageHeight)
{

    QImage OutImage(ImageWidth, ImageHeight, QImage::Format_Indexed8);//创建同样大小深度为8(即每个像素占8位)的空白图
    unsigned char * OutData = OutImage.bits();
    int RealWidthOut = OutImage.bytesPerLine();
    unsigned char* BackUpOut = OutData;
    for (int i = 0; i < ImageHeight; i++)
    {
        OutData = BackUpOut + RealWidthOut * i;
        for (int j = 0; j < ImageWidth; j++)
        {
            *OutData = 0;
            ++OutData;
        }
    }

    for (int i = 0; i < ImageHeight; i++)
    {
        OutData = BackUpOut + RealWidthOut * i;
        for (int j = 0; j < ImageWidth; j++)
        {
            *OutData = OriginalData[ImageWidth * i + j];
            ++OutData;
        }
    }

    for (int i = 0; i < 256; i++)
    {
        OutImage.setColor(i, qRgb(i, i, i));
    }
    return OutImage;
}
