#pragma once

#include <QRect>

class QPainter;

// Base class for all accent painters
class AccentPainter
{
public:
    AccentPainter() {}
    virtual ~AccentPainter() {}

    virtual void paint(QPainter *painter, const QRect& scope, const QRect& region) = 0;
};
