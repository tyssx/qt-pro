#if 1
#    include "extVariation3chtask.h"

#    include <QDebug>
//#include <opencv2/opencv.hpp>

bool ExtVariation3ChTask::first = true;

INT64 ExtVariation3ChTask::imgHandle;

ExtVariation3ChTask::ExtVariation3ChTask()
{
    m_detected.setTask(this);
}

ExtVariation3ChTask::~ExtVariation3ChTask()
{
}

// ExtVariation3ChTask *ExtVariation3ChTask::clone() const
//{
//    ExtVariation3ChTask *task = new ExtVariation3ChTask(*this);
//    return task;
//}
bool ExtVariation3ChTask::getSegmentRegion()
{
    bool result = false;
    //输入ROI四个点
    CxPointArray roiArray;

    Stroke srk = m_roi.stroke();
    int sz     = srk.size();
    for (int i = 0; i < sz; i++)
    {
        CxPointD pd;
        pd.PutX(srk[i].x);
        pd.PutY(srk[i].y);
        if (i < 4) roiArray.push_back(pd);
    }
    //    qDebug() << "roiArray size" << roiArray.GetSize();
    if (roiArray.GetSize() != 4) return false;

    //清空缓存
    m_detected.m_segment.clear();
    m_vctCxPta.clear();
    m_edges.SetSize(0);
    segCount = 0;

    CxPointArray segment, centerArea;
    /*
         在输入图像img的指定 ROI内部做图像分割
         输出轮廓，和相应的中心位置已经面积
         segma,分割因子，数值1.0 - 20.0, 推荐值 3（数值大，边缘细节更少，噪声也少）
         erosion，分割后进行腐蚀
         min_area,最小分割面积
         channel 0:R  1:G  2:B  3:Gray
    */
    qDebug() << "detect2 imgHandle" << imgHandle;
    //    IplImage *ipl = vxGetIplimage(imgHandle);
    bool ok = vxSegmentImage(imgHandle,
        roiArray,
        m_param.segma,
        m_param.erosion,
        m_param.minarea,
        m_param.channel,
        segment,
        centerArea);
    qDebug() << "vxSegmentImage ok is" << ok;
    if (ok)
    {
        qDebug() << "segment size" << segment.GetSize();
        qDebug() << "CenterArea size" << centerArea.GetSize();
        emit sendEdges(roiArray, segment, centerArea);
        int size = segment.GetSize();
        qDebug() << "segment size" << size;
        if (size < 1) { goto END; }
        else
        {
            result = true;
        }
        std::vector<Stroke> ctr;
        Stroke outSrk;
        CxPointArray cxpta;
        bool first = true;
        //获取到的轮廓再做分割作为不同区域，每个区域开始点GetTheta为1.0，其他0.0
        for (unsigned int i = 0; i < size; i++)
        {
            CxPointD &pd = segment[i];
            if (!pd.IsActive()) continue;

            int theta = pd.GetTheta();
            if (theta == 1 && !first)
            {
                qDebug() << "theta == 1 && !first";
                ctr.push_back(outSrk);
                m_vctCxPta.push_back(cxpta);
                outSrk.clear();
                cxpta.SetSize(0);
            }
            else if (theta == 1 && first)
            {
                qDebug() << "theta == 1 && first";
                first = false;
            }
            outSrk.push_back({float(pd.GetX()), float(pd.GetY())});
            cxpta.push_back(pd);
        }
        ctr.push_back(outSrk);
        m_vctCxPta.push_back(cxpta);
        qDebug() << "detect2 ctr size" << ctr.size();

        size = centerArea.GetSize(); //
        qDebug() << "detect2 centerArea size" << size;

        segCount = size;
        m_detected.m_segment.setContour(ctr);
        //        m_detected.m_pass = true; //检测通过
        //        for (unsigned int i = 0; i < size; i++)
        //        {
        //            CxPointD &pd = centerArea[i];
        //            if (!pd.IsActive()) continue;
        //            double center = pd.GetTheta();
        //            qDebug() << "pd" << i << "xy" << pd.GetX() <<
        //            pd.GetY() << "center Area" << center;
        //        }
    }
END:
    //    if (!result)
    //    {
    //        // roi仿射轮廓
    //        m_detected.setRoiAffine({mapVector(m_roi.stroke())});
    //        m_detected.m_pass = false; //检测不通过
    //    }
    return result;
}

bool ExtVariation3ChTask::detect(int mode)
{
    bool result = false;
    if (mode == 2)
    {
        //        result = this->getContourRegion();
        this->getSegmentRegion();
    }
    return result;
}

bool ExtVariation3ChTask::setParam(void *p, int size)
{
    if (size == sizeof(m_param))
    {
        m_param = *reinterpret_cast<Param *>(p);
        return true;
    }
    else
    {
        return false;
    }
}

bool ExtVariation3ChTask::setRoi(void *p, int size)
{
    if (size == sizeof(m_roi))
    {
        m_roi = *reinterpret_cast<Roi *>(p);
        return true;
    }
    else
    {
        return false;
    }
}

// bool ExtVariation3ChTask::setSimpParam(void *p, int size)
//{
//    if (size == sizeof(m_simpParam))
//    {
//        m_simpParam = *reinterpret_cast<SimpParam *>(p);
//        return true;
//    }
//    else
//    {
//        return false;
//    }
//}
#endif
