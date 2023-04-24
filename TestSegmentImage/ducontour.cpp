#include "ducontour.h"

DuContour::DuContour()
{
}

void DuContour::clear()
{
    m_contour.clear();
}

void DuContour::setContour(const std::vector<Stroke> &ctr)
{
    m_contour = ctr;
}
