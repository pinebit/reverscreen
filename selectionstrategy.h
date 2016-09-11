#pragma once

#include <QRect>
#include <QPoint>


// Interface for selection strategy
class SelectionStrategy
{
public:
    virtual ~SelectionStrategy() {}

    virtual QRect hint(const QPoint& point) = 0;
    virtual QPoint begin(const QPoint& point) = 0;
    virtual QPoint end(const QRect& rect) = 0;
    virtual QRect adjust(const QRect& rect) = 0;
};
