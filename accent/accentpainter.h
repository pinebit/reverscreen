#pragma once

#include <QRect>
#include <QColor>


class QPainter;

// Base class for all accent painters
class AccentPainter
{
public:
    AccentPainter() {}
    virtual ~AccentPainter() {}

    void setAccentParameters(QRect region, QColor color);

    virtual void paint(QPainter* painter, QRect scope) = 0;

protected:
    QRect _region;
    QColor _color;
};
