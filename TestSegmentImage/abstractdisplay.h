#ifndef ABSTRACTDISPLAY_H
#define ABSTRACTDISPLAY_H

#include "dutypes.h"

class AbstractDisplay
{
public:
    AbstractDisplay() {}
    virtual void setExtEdge(const std::vector<Stroke> &ctr)    = 0;
    virtual void setExtSegment(const std::vector<Stroke> &ctr) = 0;
};

#endif // ABSTRACTDISPLAY_H
