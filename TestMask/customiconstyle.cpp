#include "customiconstyle.h"

CustomIconStyle::CustomIconStyle()
{
    mSize = 20;
}

CustomIconStyle::~CustomIconStyle()
{
}

void CustomIconStyle::SetCustomSize(int nSize)
{
    mSize = nSize;
}

int CustomIconStyle::pixelMetric(
    PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int s = QCommonStyle::pixelMetric(metric, option, widget);

    if (metric == QStyle::PM_SmallIconSize) { s = mSize; }

    return s;
}
