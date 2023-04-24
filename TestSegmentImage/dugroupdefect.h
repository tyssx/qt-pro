#ifndef DUGROUPDEFECT_H
#define DUGROUPDEFECT_H

#include "abstractdisplay.h"
#include "ducontour.h"

class ExtVariation3ChTask;

class DuGroupDefect
{
public:
    DuGroupDefect();
    enum
    {
        Type = 110
    };
    int type() const { return Type; }
    void setTask(ExtVariation3ChTask *task) { m_task = task; }
    void updateDisplay(AbstractDisplay *ob);

    ExtVariation3ChTask *m_task;
    DuContour m_edges;   //有效轮廓
    DuContour m_segment; //分割区域轮廓
};

#endif // DUGROUPDEFECT_H
