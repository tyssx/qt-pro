#ifndef DUCONTOUR_H
#define DUCONTOUR_H

#include "dutypes.h"

class DuContour
{
public:
    DuContour();

    void clear();
    void setContour(const std::vector<Stroke> &);
    const std::vector<Stroke> &contour() const { return m_contour; }
    std::vector<Stroke> &contour() { return m_contour; }

private:
    std::vector<Stroke> m_contour; //轮廓
};

#endif // DUCONTOUR_H
