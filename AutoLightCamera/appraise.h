#ifndef APPRAISE_H
#define APPRAISE_H

#include "measure/MeasureAssist.h"

class Appraise
{
public:
    Appraise();
    //    static int MaxFunc(uchar *data, int width, int height, int roiLeft, int roiTop, int roiWidth, int roiHeight);
    int MaxFunc(uchar *data, int width, int height, int roiLeft, int roiTop, int roiWidth, int roiHeight);
};

#endif // APPRAISE_H
