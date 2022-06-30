#include "appraise.h"

Appraise::Appraise()
{
}

int Appraise::MaxFunc(uchar *data, int width, int height, int roiLeft, int roiTop, int roiWidth, int roiHeight)
{
    zcRect roi;
    roi.Left   = roiLeft;
    roi.Top    = roiTop;
    roi.Width  = roiWidth;
    roi.Height = roiHeight;
    int ret    = ImageHistValue(data, width, height, roi);
    return ret;
}
