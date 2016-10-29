#pragma once

#include <QRect>
#include <QPoint>


// Interface for selection strategy
class SnapAssistant
{
public:
    virtual ~SnapAssistant() {}

    // finds largest enclosing region
    virtual QRect snap(const QRect& rect) = 0;
};
