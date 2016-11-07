#pragma once

#include <QRect>
#include <QPoint>
#include <QPolygon>


// Interface for selection strategy
class SnapAssistant
{
public:
    virtual ~SnapAssistant() {}

    // finds largest enclosing region
    virtual QRect snap(const QRect& rect) = 0;

    // builds convex hull from objects crossed by the given track
    virtual QPolygon marker(const QList<QPoint>& track) = 0;
};
