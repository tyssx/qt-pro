#include "dugroupdefect.h"

#include "extvariation3chtask.h"

DuGroupDefect::DuGroupDefect()
{
}

void DuGroupDefect::updateDisplay(AbstractDisplay *ob)
{
    ob->setExtSegment(m_segment.contour());
}
