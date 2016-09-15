#pragma once

#include <QRect>
#include <QPoint>


// Interface for selection strategy
class SnapAssistant
{
public:
    virtual ~SnapAssistant() {}

    // snaps a selection start point
    virtual QPoint begin(const QPoint& point) = 0;
    // snaps a selection rectange
    virtual QPoint end(const QRect& rect) = 0;
};
